#!/bin/sh
#SBATCH --partition=ee4c07
#SBATCH --qos=ee4c07
#SBATCH --time=0:01:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=1024
#SBATCH --gres=gpu
#SBATCH --mail-type=FAIL

cd baseline/src/
mkdir debug
cd debug
cmake3 ..
make
srun ./matmult-benchmark -o 2
