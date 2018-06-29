#!/bin/env bash

#SBATCH --partition=shared-gpu
#SBATCH --time=00:10:00
#SBATCH --gres=gpu:1

module load CUDA

# see here for more samples:

# if you need to know the allocated CUDA device, you can obtain it here:
# echo $CUDA_VISIBLE_DEVICES
# echo $LD_LIBRARY_PATH

srun ./bin/lbmSimple -p -l -i 1000 -x 100 -y 100 -z 100
