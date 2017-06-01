/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2015 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file
 * Descriptor for nearest-neighbor 3D lattices -- generic code.
 **/
#ifndef EXTENDED_NEIGHBORHOOD_LATTICES_3D_HH
#define EXTENDED_NEIGHBORHOOD_LATTICES_3D_HH

#include "latticeBoltzmann/nearestNeighborLattices3D.h"

namespace plb {

namespace descriptors {
    
    // D3Q121 ////////////////////////////////////////////////////////////
    
    template<typename T>
    const T D3Q121Constants<T>::invD = (T)1 / (T) d;
    
    template<typename T>
    const int D3Q121Constants<T>::vicinity = 3;
    
    template<typename T>
    const int D3Q121Constants<T>::c[D3Q121Constants<T>::q][D3Q121Constants<T>::d] =
    {
        {0, 0, 0}, 
        {-3, -3, -3}, {-3, -3, 3}, {-3, -2, 0}, {-3, -1, -1}, {-3, -1, 1}, 
        {-3, 0, -2}, {-3, 0, 0}, {-3, 0, 2}, {-3, 1, -1}, {-3, 1, 1}, 
        {-3, 2, 0}, {-3, 3, -3}, {-3, 3, 3}, {-2, -3, 0}, {-2, -2, -2}, 
        {-2, -2, 0}, {-2, -2, 2}, {-2, -1, 0}, {-2, 0, -3}, {-2, 0, -2}, 
        {-2, 0, -1}, {-2, 0, 1}, {-2, 0, 2}, {-2, 0, 3}, {-2, 1, 0}, 
        {-2, 2, -2}, {-2, 2, 0}, {-2, 2, 2}, {-2, 3, 0}, {-1, -3, -1}, 
        {-1, -3, 1}, {-1, -2, 0}, {-1, -1, -3}, {-1, -1, -1}, {-1, -1, 1}, 
        {-1, -1, 3}, {-1, 0, -2}, {-1, 0, 0}, {-1, 0, 2}, {-1, 1, -3}, 
        {-1, 1, -1}, {-1, 1, 1}, {-1, 1, 3}, {-1, 2, 0}, {-1, 3, -1}, 
        {-1, 3, 1}, {0, -3, -2}, {0, -3, 0}, {0, -3, 2}, {0, -2, -3}, 
        {0, -2, -2}, {0, -2, -1}, {0, -2, 1}, {0, -2, 2}, {0, -2, 3}, 
        {0, -1, -2}, {0, -1, 0}, {0, -1, 2}, {0, 0, -3}, {0, 0, -1}, 
        {3, 3, 3}, {3, 3, -3}, {3, 2, 0}, {3, 1, 1}, {3, 1, -1}, 
        {3, 0, 2}, {3, 0, 0}, {3, 0, -2}, {3, -1, 1}, {3, -1, -1}, 
        {3, -2, 0}, {3, -3, 3}, {3, -3, -3}, {2, 3, 0}, {2, 2, 2}, 
        {2, 2, 0}, {2, 2, -2}, {2, 1, 0}, {2, 0, 3}, {2, 0, 2}, 
        {2, 0, 1}, {2, 0, -1}, {2, 0, -2}, {2, 0, -3}, {2, -1, 0}, 
        {2, -2, 2}, {2, -2, 0}, {2, -2, -2}, {2, -3, 0}, {1, 3, 1}, 
        {1, 3, -1}, {1, 2, 0}, {1, 1, 3}, {1, 1, 1}, {1, 1, -1}, 
        {1, 1, -3}, {1, 0, 2}, {1, 0, 0}, {1, 0, -2}, {1, -1, 3}, 
        {1, -1, 1}, {1, -1, -1}, {1, -1, -3}, {1, -2, 0}, {1, -3, 1}, 
        {1, -3, -1}, {0, 3, 2}, {0, 3, 0}, {0, 3, -2}, {0, 2, 3}, 
        {0, 2, 2}, {0, 2, 1}, {0, 2, -1}, {0, 2, -2}, {0, 2, -3}, 
        {0, 1, 2}, {0, 1, 0}, {0, 1, -2}, {0, 0, 3}, {0, 0, 1}, 
    };


    
    template<typename T>
    const int D3Q121Constants<T>::cNormSqr[D3Q121Constants<T>::q] =
    {
        0, 
        27, 27, 13, 11, 11, 13, 9, 13, 11, 11, 13, 
        27, 27, 13, 12, 8, 12, 5, 13, 8, 5, 5, 8, 
        13, 5, 12, 8, 12, 13, 11, 11, 5, 11, 3, 3, 
        11, 5, 1, 5, 11, 3, 3, 11, 5, 11, 11, 
        13, 9, 13, 13, 8, 5, 5, 8, 13, 5, 1, 
        5, 9, 1, 27, 27, 13, 11, 11, 13, 9, 13, 
        11, 11, 13, 27, 27, 13, 12, 8, 12, 5, 
        13, 8, 5, 5, 8, 13, 5, 12, 8, 12, 13, 
        11, 11, 5, 11, 3, 3, 11, 5, 1, 5, 11, 
        3, 3, 11, 5, 11, 11, 13, 9, 13, 13, 
        8, 5, 5, 8, 13, 5, 1, 5, 9, 1, 
    };
    
    template<typename T>
    const T D3Q121Constants<T>::t[D3Q121Constants<T>::q] =
    {
        3.059162202948600642469372949253824201326e-02, 
        6.928750896386028525506511533901772605647e-07, 
        6.928750896386028525506511533901772605647e-07, 
        1.431862411548029405509826922297073180664e-05, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        1.431862411548029405509826922297073180664e-05, 
        3.247475270880738129679305570682130634477e-04, 
        1.431862411548029405509826922297073180664e-05, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        1.431862411548029405509826922297073180664e-05, 
        6.928750896386028525506511533901772605647e-07, 
        6.928750896386028525506511533901772605647e-07, 
        1.431862411548029405509826922297073180664e-05, 
        1.810217515763742375901616857556491604089e-04, 
        4.281835936810840661857590214080748494426e-04, 
        1.810217515763742375901616857556491604089e-04, 
        6.111023366833424322414921694060723126594e-03, 
        1.431862411548029405509826922297073180664e-05, 
        4.281835936810840661857590214080748494426e-04, 
        6.111023366833424322414921694060723126594e-03, 
        6.111023366833424322414921694060723126594e-03, 
        4.281835936810840661857590214080748494426e-04, 
        1.431862411548029405509826922297073180664e-05, 
        6.111023366833424322414921694060723126594e-03, 
        1.810217515763742375901616857556491604089e-04, 
        4.281835936810840661857590214080748494426e-04, 
        1.810217515763742375901616857556491604089e-04, 
        1.431862411548029405509826922297073180664e-05, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        6.111023366833424322414921694060723126594e-03, 
        1.068340024593910949171102482709870577810e-04, 
        2.752500532563812386479902897409233864522e-02, 
        2.752500532563812386479902897409233864522e-02, 
        1.068340024593910949171102482709870577810e-04, 
        6.111023366833424322414921694060723126594e-03, 
        9.851595103726339186467408722380536926020e-02, 
        6.111023366833424322414921694060723126594e-03, 
        1.068340024593910949171102482709870577810e-04, 
        2.752500532563812386479902897409233864522e-02, 
        2.752500532563812386479902897409233864522e-02, 
        1.068340024593910949171102482709870577810e-04, 
        6.111023366833424322414921694060723126594e-03, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        1.431862411548029405509826922297073180664e-05, 
        3.247475270880738129679305570682130634477e-04, 
        1.431862411548029405509826922297073180664e-05, 
        1.431862411548029405509826922297073180664e-05, 
        4.281835936810840661857590214080748494426e-04, 
        6.111023366833424322414921694060723126594e-03, 
        6.111023366833424322414921694060723126594e-03, 
        4.281835936810840661857590214080748494426e-04, 
        1.431862411548029405509826922297073180664e-05, 
        6.111023366833424322414921694060723126594e-03, 
        9.851595103726339186467408722380536926020e-02, 
        6.111023366833424322414921694060723126594e-03, 
        3.247475270880738129679305570682130634477e-04, 
        9.851595103726339186467408722380536926020e-02, 
        6.928750896386028525506511533901772605647e-07, 
        6.928750896386028525506511533901772605647e-07, 
        1.431862411548029405509826922297073180664e-05, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        1.431862411548029405509826922297073180664e-05, 
        3.247475270880738129679305570682130634477e-04, 
        1.431862411548029405509826922297073180664e-05, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        1.431862411548029405509826922297073180664e-05, 
        6.928750896386028525506511533901772605647e-07, 
        6.928750896386028525506511533901772605647e-07, 
        1.431862411548029405509826922297073180664e-05, 
        1.810217515763742375901616857556491604089e-04, 
        4.281835936810840661857590214080748494426e-04, 
        1.810217515763742375901616857556491604089e-04, 
        6.111023366833424322414921694060723126594e-03, 
        1.431862411548029405509826922297073180664e-05, 
        4.281835936810840661857590214080748494426e-04, 
        6.111023366833424322414921694060723126594e-03, 
        6.111023366833424322414921694060723126594e-03, 
        4.281835936810840661857590214080748494426e-04, 
        1.431862411548029405509826922297073180664e-05, 
        6.111023366833424322414921694060723126594e-03, 
        1.810217515763742375901616857556491604089e-04, 
        4.281835936810840661857590214080748494426e-04, 
        1.810217515763742375901616857556491604089e-04, 
        1.431862411548029405509826922297073180664e-05, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        6.111023366833424322414921694060723126594e-03, 
        1.068340024593910949171102482709870577810e-04, 
        2.752500532563812386479902897409233864522e-02, 
        2.752500532563812386479902897409233864522e-02, 
        1.068340024593910949171102482709870577810e-04, 
        6.111023366833424322414921694060723126594e-03, 
        9.851595103726339186467408722380536926020e-02, 
        6.111023366833424322414921694060723126594e-03, 
        1.068340024593910949171102482709870577810e-04, 
        2.752500532563812386479902897409233864522e-02, 
        2.752500532563812386479902897409233864522e-02, 
        1.068340024593910949171102482709870577810e-04, 
        6.111023366833424322414921694060723126594e-03, 
        1.068340024593910949171102482709870577810e-04, 
        1.068340024593910949171102482709870577810e-04, 
        1.431862411548029405509826922297073180664e-05, 
        3.247475270880738129679305570682130634477e-04, 
        1.431862411548029405509826922297073180664e-05, 
        1.431862411548029405509826922297073180664e-05, 
        4.281835936810840661857590214080748494426e-04, 
        6.111023366833424322414921694060723126594e-03, 
        6.111023366833424322414921694060723126594e-03, 
        4.281835936810840661857590214080748494426e-04, 
        1.431862411548029405509826922297073180664e-05, 
        6.111023366833424322414921694060723126594e-03, 
        9.851595103726339186467408722380536926020e-02, 
        6.111023366833424322414921694060723126594e-03, 
        3.247475270880738129679305570682130634477e-04, 
        9.851595103726339186467408722380536926020e-02 
    };

    
    template<typename T>
    const T D3Q121Constants<T>::cs2 = 1./ ( 1.19697977039307435897238846385* 1.19697977039307435897238846385);
    
    template<typename T>
    const T D3Q121Constants<T>::invCs2 =  1.19697977039307435897238846385* 1.19697977039307435897238846385;
    
    template<typename T>
    const char D3Q121Descriptor<T>::name[] = "D3Q121";
    
    template<typename T>
    const char ForcedD3Q121Descriptor<T>::name[] = "ForcedD3Q121";

}  // namespace descriptors

namespace descriptors {
    
    // D3Q39 ////////////////////////////////////////////////////////////
    
    template<typename T>
    const T D3Q39Constants<T>::invD = (T)1 / (T) d;
    
    template<typename T>
    const int D3Q39Constants<T>::vicinity = 3;
    
    template<typename T>
    const int D3Q39Constants<T>::c[D3Q39Constants<T>::q][D3Q39Constants<T>::d] =
    {
         {0, 0, 0},

         {-3, 0, 0},
         {-2, -2, 0}, {-2, 0, -2}, {-2, 0, 0}, {-2, 0, 2}, {-2, 2, 0},
         {-1, -1, -1}, {-1, -1, 1}, {-1, 0, 0}, {-1, 1, -1}, {-1, 1, 1},
         {0, -3, 0}, {0, -2, -2}, {0, -2, 0}, {0, -2, 2},
         {0, -1, 0}, {0, 0, -3}, {0, 0, -2}, {0, 0, -1},

         {3, 0, 0},
         {2, 2, 0}, {2, 0, 2}, {2, 0, 0}, {2, 0, -2}, {2, -2, 0},
         {1, 1, 1}, {1, 1, -1}, {1, 0, 0}, {1, -1, 1}, {1, -1, -1},
         {0, 3, 0}, {0, 2, 2}, {0, 2, 0}, {0, 2, -2},
         {0, 1, 0}, {0, 0, 3}, {0, 0, 2}, {0, 0, 1}

    };
    
    template<typename T>
    const int D3Q39Constants<T>::cNormSqr[D3Q39Constants<T>::q] =
    {0, 9, 8, 8, 4, 8, 8, 3, 3, 1, 3, 3, 9, 8, 4, 8, 1, 9, 4, 1, 9, 8, 8, 4, 8, 8, 3, 3, 1, 3, 3, 9, 8, 4, 8, 1, 9, 4, 1};

    template<typename T>
    const T D3Q39Constants<T>::t[D3Q39Constants<T>::q] =
    {
       1. / 12.,
       1. / 1620.,
       1. / 432.,
       1. / 432.,
       2. / 135.,
       1. / 432.,
       1. / 432.,
       1. / 27.,
       1. / 27.,
       1. / 12.,
       1. / 27.,
       1. / 27.,
       1. / 1620.,
       1. / 432.,
       2. / 135.,
       1. / 432.,
       1. / 12.,
       1. / 1620.,
       2. / 135.,
       1. / 12.,
       1. / 1620.,
       1. / 432.,
       1. / 432.,
       2. / 135.,
       1. / 432.,
       1. / 432.,
       1. / 27.,
       1. / 27.,
       1. / 12.,
       1. / 27.,
       1. / 27.,
       1. / 1620.,
       1. / 432.,
       2. / 135.,
       1. / 432.,
       1. / 12.,
       1. / 1620.,
       2. / 135.,
       1. / 12.
    };
    
    template<typename T>
    const T D3Q39Constants<T>::cs2 = 2./3.;
    
    template<typename T>
    const T D3Q39Constants<T>::invCs2 =  3./2.;
    
    template<typename T>
    const char D3Q39Descriptor<T>::name[] = "D3Q39";
    
    template<typename T>
    const char ForcedD3Q39Descriptor<T>::name[] = "ForcedD3Q39";
}

}  // namespace plb

#endif


// The ordering of these constants was computed with the following Python script:
/*
from fractions import Fraction as F

class Vel:
    def __init__(self, ci, ti):
        self.ci = ci
        self.ti = ti
    def isZero(self):
        return self.ci[0]==0 and self.ci[1]==0 and self.ci[2]==0
    def opp(self):
        """Opposite directions (negative velocity)"""
        return Vel([-self.ci[0],-self.ci[1],-self.ci[2]],self.ti)
    def __cmp__(self, other):
        """Compare two velocities to get an ordering according to the Palabos
           rule"""
        # Make sure zero velocity comes first.
        if self.isZero():
            if other.isZero():
                return 0
            else:
                return -1
        if other.isZero():
            return +1
        # Apply the ordering rule to the lower-half of velocities
        # only. The upper half is defined by being equal to the
        # lower half, with opposite sign.
        self_lower = self.__cmp_half__(Vel([0,0,0],1.))==-1
        other_lower = other.__cmp_half__(Vel([0,0,0],1.))==-1
        if self_lower:
            if other_lower:
                return self.__cmp_half__(other)
            else:
                return -1
        else:
            if not other_lower:
                return self.opp().__cmp_half__(other.opp())
            else:
                return +1
    def __cmp_half__(self, other):
        """ Ordering rule for the lower half of velocities. """
        if self.ci[0] < other.ci[0]:
            return -1
        elif self.ci[0]==other.ci[0]:
            if self.ci[1] < other.ci[1]:
                return -1
            elif self.ci[1] == other.ci[1]:
                if self.ci[2] < other.ci[2]:
                    return -1
                elif self.ci[2] == other.ci[2]:
                    return 0
        return +1
    def __repr__(self):
        """ Pretty-print the content. """
        cnorm_sqr = self.ci[0]**2 + self.ci[1]**2 + self.ci[2]**2
        return "{"+self.ci.__repr__()+";"+cnorm_sqr.__repr__()+";"+self.ti.__repr__()+"}"


def d3q39():
    b = [ Vel( [ 0,  0,  0], F(1,12)),

          Vel( [ 1,  0,  0], F(1,12)),
          Vel( [-1,  0,  0], F(1,12)),
          Vel( [ 0,  1,  0], F(1,12)),
          Vel( [ 0, -1,  0], F(1,12)),
          Vel( [ 0,  0,  1], F(1,12)),
          Vel( [ 0,  0, -1], F(1,12)),

          Vel( [-1, -1, -1], F(1,27)),
          Vel( [-1, -1,  1], F(1,27)),
          Vel( [-1,  1, -1], F(1,27)),
          Vel( [-1,  1,  1], F(1,27)),
          Vel( [ 1, -1, -1], F(1,27)),
          Vel( [ 1, -1,  1], F(1,27)),
          Vel( [ 1,  1, -1], F(1,27)),
          Vel( [ 1,  1,  1], F(1,27)),

          Vel( [ 2,  0,  0], F(2,135)),
          Vel( [-2,  0,  0], F(2,135)),
          Vel( [ 0,  2,  0], F(2,135)),
          Vel( [ 0, -2,  0], F(2,135)),
          Vel( [ 0,  0,  2], F(2,135)),
          Vel( [ 0,  0, -2], F(2,135)),

          Vel( [ 2,  2,  0], F(1,432)),
          Vel( [ 2, -2,  0], F(1,432)),
          Vel( [-2,  2,  0], F(1,432)),
          Vel( [-2, -2,  0], F(1,432)),
          Vel( [ 2,  0,  2], F(1,432)),
          Vel( [ 2,  0, -2], F(1,432)),
          Vel( [-2,  0,  2], F(1,432)),
          Vel( [-2,  0, -2], F(1,432)),
          Vel( [ 0,  2,  2], F(1,432)),
          Vel( [ 0,  2, -2], F(1,432)),
          Vel( [ 0, -2,  2], F(1,432)),
          Vel( [ 0, -2, -2], F(1,432)),

          Vel( [ 3,  0,  0], F(1,1620)),
          Vel( [-3,  0,  0], F(1,1620)),
          Vel( [ 0,  3,  0], F(1,1620)),
          Vel( [ 0, -3,  0], F(1,1620)),
          Vel( [ 0,  0,  3], F(1,1620)),
          Vel( [ 0,  0, -3], F(1,1620)) ]

    b.sort()

    print(b)

    print(Vel([-3,-3,-3],1.).__cmp_half__(Vel([-3,-3,3],1.)))

def d3q121():
    cl = 1
    b= [ Vel([0, 0, 0], 0.03059162202948600642469), 

         Vel([ cl, 0, 0], 0.09851595103726339186467),
         Vel([-cl, 0, 0], 0.09851595103726339186467),
         Vel([  0, cl, 0], 0.09851595103726339186467),
         Vel([  0,-cl, 0], 0.09851595103726339186467),
         Vel([  0, 0, cl], 0.09851595103726339186467),
         Vel([  0, 0,-cl], 0.09851595103726339186467),
         
         Vel([ cl, cl, cl], 0.02752500532563812386479),
         Vel([ cl, cl,-cl], 0.02752500532563812386479),
         Vel([ cl,-cl, cl], 0.02752500532563812386479),
         Vel([ cl,-cl,-cl], 0.02752500532563812386479),
         Vel([-cl, cl, cl], 0.02752500532563812386479),
         Vel([-cl, cl,-cl], 0.02752500532563812386479),
         Vel([-cl,-cl, cl], 0.02752500532563812386479),
         Vel([-cl,-cl,-cl], 0.02752500532563812386479),
         
         Vel([ cl, 2*cl, 0], 0.00611102336683342432241),
         Vel([ cl,-2*cl, 0], 0.00611102336683342432241),
         Vel([-cl, 2*cl, 0], 0.00611102336683342432241),
         Vel([-cl,-2*cl, 0], 0.00611102336683342432241),
         Vel([ cl, 0, 2*cl], 0.00611102336683342432241),
         Vel([ cl, 0,-2*cl], 0.00611102336683342432241),
         Vel([-cl, 0, 2*cl], 0.00611102336683342432241),
         Vel([-cl, 0,-2*cl], 0.00611102336683342432241),
         Vel([ 0, cl, 2*cl], 0.00611102336683342432241),
         Vel([ 0, cl,-2*cl], 0.00611102336683342432241),
         Vel([ 0,-cl, 2*cl], 0.00611102336683342432241),
         Vel([ 0,-cl,-2*cl], 0.00611102336683342432241),
         Vel([ 0, 2*cl, cl], 0.00611102336683342432241),
         Vel([ 0, 2*cl,-cl], 0.00611102336683342432241),
         Vel([ 0,-2*cl, cl], 0.00611102336683342432241),
         Vel([ 0,-2*cl,-cl], 0.00611102336683342432241),
         Vel([ 2*cl, 0, cl], 0.00611102336683342432241),
         Vel([ 2*cl, 0,-cl], 0.00611102336683342432241),
         Vel([-2*cl, 0, cl], 0.00611102336683342432241),
         Vel([-2*cl, 0,-cl], 0.00611102336683342432241),
         Vel([ 2*cl, cl, 0], 0.00611102336683342432241),
         Vel([ 2*cl,-cl, 0], 0.00611102336683342432241),
         Vel([-2*cl, cl, 0], 0.00611102336683342432241),
         Vel([-2*cl,-cl, 0], 0.00611102336683342432241),
         
         Vel([ 2*cl, 2*cl, 0], 0.00042818359368108406618),
         Vel([ 2*cl,-2*cl, 0], 0.00042818359368108406618),
         Vel([-2*cl, 2*cl, 0], 0.00042818359368108406618),
         Vel([-2*cl,-2*cl, 0], 0.00042818359368108406618),
         Vel([ 2*cl, 0, 2*cl], 0.00042818359368108406618),
         Vel([ 2*cl, 0,-2*cl], 0.00042818359368108406618),
         Vel([-2*cl, 0, 2*cl], 0.00042818359368108406618),
         Vel([-2*cl, 0,-2*cl], 0.00042818359368108406618),
         Vel([ 0, 2*cl, 2*cl], 0.00042818359368108406618),
         Vel([ 0, 2*cl,-2*cl], 0.00042818359368108406618),
         Vel([ 0,-2*cl, 2*cl], 0.00042818359368108406618),
         Vel([ 0,-2*cl,-2*cl], 0.00042818359368108406618),
         
         
         Vel([ 3*cl, 0, 0], 0.00032474752708807381296),
         Vel([-3*cl, 0, 0], 0.00032474752708807381296),
         Vel([ 0, 3*cl, 0], 0.00032474752708807381296),
         Vel([ 0,-3*cl, 0], 0.00032474752708807381296),
         Vel([ 0, 0, 3*cl], 0.00032474752708807381296),
         Vel([ 0, 0,-3*cl], 0.00032474752708807381296),
         
         Vel([ 2*cl, 3*cl, 0], 0.00001431862411548029405),
         Vel([ 2*cl,-3*cl, 0], 0.00001431862411548029405), 
         Vel([-2*cl, 3*cl, 0], 0.00001431862411548029405),
         Vel([-2*cl,-3*cl, 0], 0.00001431862411548029405), 
         Vel([ 2*cl, 0, 3*cl], 0.00001431862411548029405), 
         Vel([ 2*cl, 0,-3*cl], 0.00001431862411548029405), 
         Vel([-2*cl, 0, 3*cl], 0.00001431862411548029405), 
         Vel([-2*cl, 0,-3*cl], 0.00001431862411548029405), 
         Vel([ 0, 2*cl, 3*cl], 0.00001431862411548029405), 
         Vel([ 0, 2*cl,-3*cl], 0.00001431862411548029405), 
         Vel([ 0,-2*cl, 3*cl], 0.00001431862411548029405), 
         Vel([ 0,-2*cl,-3*cl], 0.00001431862411548029405), 
         Vel([ 3*cl, 2*cl, 0], 0.00001431862411548029405),
         Vel([ 3*cl,-2*cl, 0], 0.00001431862411548029405), 
         Vel([-3*cl, 2*cl, 0], 0.00001431862411548029405),
         Vel([-3*cl,-2*cl, 0], 0.00001431862411548029405), 
         Vel([ 3*cl, 0, 2*cl], 0.00001431862411548029405), 
         Vel([ 3*cl, 0,-2*cl], 0.00001431862411548029405), 
         Vel([-3*cl, 0, 2*cl], 0.00001431862411548029405), 
         Vel([-3*cl, 0,-2*cl], 0.00001431862411548029405), 
         Vel([ 0, 3*cl, 2*cl], 0.00001431862411548029405), 
         Vel([ 0, 3*cl,-2*cl], 0.00001431862411548029405), 
         Vel([ 0,-3*cl, 2*cl], 0.00001431862411548029405), 
         Vel([ 0,-3*cl,-2*cl], 0.00001431862411548029405), 
         
         Vel([ 2*cl, 2*cl, 2*cl], 0.00018102175157637423759),
         Vel([ 2*cl, 2*cl,-2*cl], 0.00018102175157637423759),
         Vel([ 2*cl,-2*cl, 2*cl], 0.00018102175157637423759),
         Vel([ 2*cl,-2*cl,-2*cl], 0.00018102175157637423759),
         Vel([-2*cl, 2*cl, 2*cl], 0.00018102175157637423759),
         Vel([-2*cl, 2*cl,-2*cl], 0.00018102175157637423759),
         Vel([-2*cl,-2*cl, 2*cl], 0.00018102175157637423759),
         Vel([-2*cl,-2*cl,-2*cl], 0.00018102175157637423759),
         
         Vel([ cl, 3*cl, 0], 0.00010683400245939109491),
         Vel([ cl,-3*cl, 0], 0.00010683400245939109491),
         Vel([-cl, 3*cl, 0], 0.00010683400245939109491),
         Vel([-cl,-3*cl, 0], 0.00010683400245939109491),
         Vel([ cl, 0, 3*cl], 0.00010683400245939109491),
         Vel([ cl, 0,-3*cl], 0.00010683400245939109491),
         Vel([-cl, 0, 3*cl], 0.00010683400245939109491),
         Vel([-cl, 0,-3*cl], 0.00010683400245939109491),
         Vel([ 0, cl, 3*cl], 0.00010683400245939109491),
         Vel([ 0, cl,-3*cl], 0.00010683400245939109491),
         Vel([ 0,-cl, 3*cl], 0.00010683400245939109491),
         Vel([ 0,-cl,-3*cl], 0.00010683400245939109491),
         Vel([ 3*cl, cl, 0], 0.00010683400245939109491),
         Vel([ 3*cl,-cl, 0], 0.00010683400245939109491),
         Vel([-3*cl, cl, 0], 0.00010683400245939109491),
         Vel([-3*cl,-cl, 0], 0.00010683400245939109491),
         Vel([ 3*cl, 0, cl], 0.00010683400245939109491),
         Vel([ 3*cl, 0,-cl], 0.00010683400245939109491),
         Vel([-3*cl, 0, cl], 0.00010683400245939109491),
         Vel([-3*cl, 0,-cl], 0.00010683400245939109491),
         Vel([ 0, 3*cl, cl], 0.00010683400245939109491),
         Vel([ 0, 3*cl,-cl], 0.00010683400245939109491),
         Vel([ 0,-3*cl, cl], 0.00010683400245939109491),
         Vel([ 0,-3*cl,-cl], 0.00010683400245939109491),
         
         Vel([ 3*cl, 3*cl, 3*cl], 0.00000069287508963860285),
         Vel([ 3*cl, 3*cl,-3*cl], 0.00000069287508963860285),
         Vel([ 3*cl,-3*cl, 3*cl], 0.00000069287508963860285),
         Vel([ 3*cl,-3*cl,-3*cl], 0.00000069287508963860285),
         Vel([-3*cl, 3*cl, 3*cl], 0.00000069287508963860285),
         Vel([-3*cl, 3*cl,-3*cl], 0.00000069287508963860285),
         Vel([-3*cl,-3*cl, 3*cl], 0.00000069287508963860285),
         Vel([-3*cl,-3*cl,-3*cl], 0.00000069287508963860285) ]
         
    b.sort()

#for i in range(121):
        #print(b[i].ci)
        #print(b[i].ti)
        #print(b[i].ci[0]*b[i].ci[0]+b[i].ci[1]*b[i].ci[1]+b[i].ci[2]*b[i].ci[2])

    print(b)



d3q39()
*/
