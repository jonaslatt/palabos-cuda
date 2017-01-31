/*!
 * \file    lbmFlowAroundCylinder.cu
 * \brief   Cuda version based on lbm_palabos_friendly (standard C).
 * \author  Adrien Python
 * \date    22.01.2017
 */

#include <libc.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <libgen.h>

#define RE       220.0       // Reynolds number
#define NX       100//420         // Numer of lattice nodes (width)
#define NY       10 //180         // Numer of lattice nodes (height)
#define LY       ((NY) - 1)  // Height of the domain in lattice units
#define CX       ((NX) / 4)  // X coordinates of the cylinder
#define CY       ((NY) / 2)  // Y coordinates of the cylinder
#define R        ((NY) / 9)  // Cylinder radius
#define ULB      0.04        // Velocity in lattice units
#define NULB     ((ULB) * (R) / (RE))   // Viscoscity in lattice units
#define OMEGA    ((double)1. / (3*(NULB)+0.5))  // Relaxation parameter

#define SQUARE(a) ((a)*(a))

typedef enum { OUT_FIN, OUT_IMG, OUT_UNK } out_mode;

typedef struct {
    bool obstacles[NX][NY];  // Should reside in lbm_consts but is too big for constant memory
    double u[NX][NY][2];
    double feq[NX][NY][9];
    double fin[NX][NY][9];
    double fout[NX][NY][9];
    double rho[NX][NY];
    double vel[NX][NY][2];
} lbm_vars;

typedef struct {
    size_t col[3][3];
    size_t opp[9];
    ssize_t v[2][9];
    double t[9];
} lbm_consts;

__constant__ lbm_consts d_consts;

const ssize_t V[][9] = {
    { 1, 1, 1, 0, 0, 0,-1,-1,-1 },
    { 1, 0,-1, 1, 0,-1, 1, 0,-1 }
};
const double T[] = { 1./36, 1./9, 1./36, 1./9, 4./9, 1./9, 1./36, 1./9, 1./36 };

#define HANDLE_ERROR(ans) (handleError((ans), __FILE__, __LINE__))
inline void handleError(cudaError_t code, const char *file, int line)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"CUDA assert: %s %s %d\n", cudaGetErrorString(code), file, line);
      exit(EXIT_FAILURE);
   }
}

#define HANDLE_KERNEL_ERROR(...) \
do {                                         \
    __VA_ARGS__;                             \
    HANDLE_ERROR( cudaPeekAtLastError() );   \
    HANDLE_ERROR( cudaDeviceSynchronize() ); \
} while(0)

/**
 * Setup: cylindrical obstacle and velocity inlet with perturbation
 * Creation of a mask with boolean values, defining the shape of the obstacle.
 */
static void initObstacles(lbm_vars* vars)
{
    for (int x = 0; x < NX; x++) {
        for (int y = 0; y < NY; y++) {
            vars->obstacles[x][y] = SQUARE(x-CX) + SQUARE(y-CY) < SQUARE(R);
        }
    }
}

/**
 * Initial velocity profile: almost zero, with a slight perturbation to trigger
 * the instability.
 */
static void initVelocity(lbm_vars* vars)
{
    for (int d = 0; d < 2; d++) {
        for (int x = 0; x < NX; x++) {
            for (int y = 0; y < NY; y++) {
                vars->vel[x][y][d] = (1-d) * ULB * (1 + 0.0001 * sin( y / (double)LY * 2 * M_PI) );
            }
        }
    }
}

static void initRho(lbm_vars* vars)
{
    for (int x = 0; x < NX; x++) {
        for (int y = 0; y < NY; y++) {
            vars->rho[x][y] = 1.0;
        }
    }
}

static void initCol(size_t* col, ssize_t v0)
{
    for (int f = 0, i = 0; f < 9 && i < 3; f++) {
        if (V[0][f] == v0) {
            col[i++] = f;
        }
    }
}

static void initOpp(size_t* opp)
{
    for (int f = 0; f < 9; f++) {
        for (int g = 0; g < 9; g++) {
            if (V[0][f] == -V[0][g] && V[1][f] == -V[1][g]) {
                opp[f] = g;
                break;
            }
        }
    }
}

#define EQUILIBRIUM_BODY(v, t) \
    do {                                                                 \
        double usqr = 3./2 * ( SQUARE(u[0]) + SQUARE(u[1]) );            \
                                                                         \
        for (int f = 0; f < 9; f++) {                                    \
            double cu = 3 * ( v[0][f] * u[0] + v[1][f] * u[1] );         \
            feq[f] = rho * t[f] * ( 1 + cu + 0.5 * SQUARE(cu) - usqr );  \
        }                                                                \
    } while(0)

__host__ static void h_equilibrium(double* feq, double rho, double* u)
{
    EQUILIBRIUM_BODY(V, T);
}

__device__ static void d_equilibrium(double* feq, double rho, double* u)
{
    EQUILIBRIUM_BODY(d_consts.v, d_consts.t);
}

__device__ static void macroscopic(double* fin, double* rho, double* u)
{
    
    *rho = u[0] = u[1] = 0;

    for (int f = 0; f < 9; f++) {
        *rho += fin[f];

        u[0] += d_consts.v[0][f] * fin[f];
        u[1] += d_consts.v[1][f] * fin[f];
    }
    
    u[0] /= *rho;
    u[1] /= *rho;
}

__global__ void lbm_right_wall(lbm_vars *d_vars)
{
    int y = threadIdx.x / NX;

    // Right wall: outflow condition.
    for (int i = 0; i < 3; i++) {
        int f = d_consts.col[2][i];
        d_vars->fin[NX-1][y][f] = d_vars->fin[NX-2][y][f];
    }
}

__global__ void lbm_macro_and_left_wall(lbm_vars *d_vars)
{
    int y = threadIdx.x / NX;
    int x = threadIdx.x % NX;

    // Compute macroscopic variables, density and velocity
    macroscopic(d_vars->fin[x][y], &d_vars->rho[x][y], d_vars->u[x][y]);
    
    // Left wall: inflow condition
    for (size_t d = 0; d < 2; d++) {
        d_vars->u[0][y][d] = d_vars->vel[0][y][d]; // TODO: collision!
    }
 }

__global__ void lbm_density(lbm_vars *d_vars)
{
    int y = threadIdx.x;
  
    // Calculate the density
    double s2 = 0, s3 = 0;
    for (size_t i = 0; i < 3; i++) {
        s2 += d_vars->fin[0][y][d_consts.col[1][i]];
        s3 += d_vars->fin[0][y][d_consts.col[2][i]];
    }
    d_vars->rho[0][y] = 1./(1 - d_vars->u[0][y][0]) * (s2 + 2*s3);
}

__global__ void lbm_equilibrium_1(lbm_vars *d_vars)
{
    int y = threadIdx.x / NX;
    int x = threadIdx.x % NX;
   
    // Compute equilibrium
    d_equilibrium(d_vars->feq[x][y], d_vars->rho[x][y], d_vars->u[x][y]);
}

__global__ void lbm_equilibrium_2(lbm_vars *d_vars)
{
    int y = threadIdx.x;

    for (size_t i = 0, f = d_consts.col[0][i]; i < 3; f = d_consts.col[0][++i]) {
        d_vars->fin[0][y][f] = d_vars->feq[0][y][f] + d_vars->fin[0][y][d_consts.opp[f]] - d_vars->feq[0][y][d_consts.opp[f]];
    }

}

__global__ void lbm_collision(lbm_vars *d_vars)
{
    int y = threadIdx.x / NX;
    int x = threadIdx.x % NX;
    
    for (size_t f = 0; f < 9; f++) {
        if (d_vars->obstacles[x][y]) {
            // Bounce-back condition for obstacle
            d_vars->fout[x][y][f] = d_vars->fin[x][y][d_consts.opp[f]];
        } else {
            // Collision step
            d_vars->fout[x][y][f] = d_vars->fin[x][y][f] - OMEGA * (d_vars->fin[x][y][f] - d_vars->feq[x][y][f]);
        }
    }

}

__global__ void lbm_streaming(lbm_vars *d_vars)
{
    int y = threadIdx.x / NX;
    int x = threadIdx.x % NX;

    // Streaming step
    for (size_t f = 0; f < 9; f++) {
        size_t x_dst = (x + NX + d_consts.v[0][f]) % NX;
        size_t y_dst = (y + NY + d_consts.v[1][f]) % NY;
        d_vars->fin[x_dst][y_dst][f] = d_vars->fout[x][y][f];
    }

}

void print_variables(lbm_vars *d_vars, lbm_vars *h_vars, double var[NX][NY][9]) {

    HANDLE_ERROR(cudaMemcpy(h_vars, d_vars, sizeof(lbm_vars), cudaMemcpyDeviceToHost));

    for (size_t x = 0; x < NX; x++) {
        for (size_t y = 0; y < NY; y++) {
            for (size_t f = 0; f < 9; ++f) {
                printf("%64.60f\n", var[x][y][f]);
            }
        }
    }
}


int main(int argc, char * const argv[])
{
    // Read arguments
    out_mode out = OUT_UNK;
    ssize_t max_iter = 0;
    
    while (optind < argc) {
        switch (getopt(argc, argv, "p:fi:")) {
            case 'f': { out = OUT_FIN; break; }
            case 'i': { max_iter = strtol(optarg, NULL, 10); break; }
            default : { goto usage; }
        }
    }
    
    // check that execution mode is set (output images or fin values)
    if (out == OUT_UNK && max_iter < 1) {
    usage:
        fprintf(stderr, "usage: %s [-f] -i <iter> \n", basename((char*)argv[0]));
        fprintf(stderr, "  -f : output populations values in stdout\n");
        fprintf(stderr, "  -i : Total number of iterations\n");
        return EXIT_FAILURE;
    }

    lbm_consts* h_consts = (lbm_consts*)malloc(sizeof(lbm_consts));
    
    initCol(h_consts->col[0],  1);
    initCol(h_consts->col[1],  0);
    initCol(h_consts->col[2], -1);
    initOpp(h_consts->opp);
    memcpy(h_consts->v, V, sizeof(V));
    memcpy(h_consts->t, T, sizeof(T));
    
    HANDLE_ERROR(cudaMemcpyToSymbol(d_consts, h_consts, sizeof(lbm_consts)));
        
    lbm_vars *h_vars = (lbm_vars*)malloc(sizeof(lbm_vars));
    initObstacles(h_vars);
    initVelocity(h_vars);
    initRho(h_vars);
    
    // Initialization of the populations at equilibrium with the given velocity.
    for (int y = 0; y < NY; y++) {
        for (int x = 0; x < NX; x++) {
            h_equilibrium(h_vars->fin[x][y], h_vars->rho[x][y], h_vars->vel[x][y]);
        }
    }
    
    lbm_vars *d_vars;
    HANDLE_ERROR(cudaMalloc(&d_vars, sizeof(lbm_vars)));
    HANDLE_ERROR(cudaMemcpy(d_vars, h_vars, sizeof(lbm_vars), cudaMemcpyHostToDevice));
    for (int time = 0; time < max_iter; time++) {
        HANDLE_KERNEL_ERROR(         lbm_right_wall<<<1,   NY>>>(d_vars));        
        HANDLE_KERNEL_ERROR(lbm_macro_and_left_wall<<<1,NX*NY>>>(d_vars));
        HANDLE_KERNEL_ERROR(            lbm_density<<<1,   NY>>>(d_vars));
        HANDLE_KERNEL_ERROR(      lbm_equilibrium_1<<<1,NX*NY>>>(d_vars));
        HANDLE_KERNEL_ERROR(      lbm_equilibrium_2<<<1,   NY>>>(d_vars));
        HANDLE_KERNEL_ERROR(          lbm_collision<<<1,NX*NY>>>(d_vars));
        HANDLE_KERNEL_ERROR(          lbm_streaming<<<1,NX*NY>>>(d_vars));
    }

    if (out == OUT_FIN) {
        print_variables(d_vars, h_vars, h_vars->fin);
    }

    free(h_consts);
    free(h_vars);
    HANDLE_ERROR(cudaFree(d_vars));
    
    return EXIT_SUCCESS;
}