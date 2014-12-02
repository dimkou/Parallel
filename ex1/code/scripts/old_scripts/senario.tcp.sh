#!/bin/bash

## Give the Job a descriptive name
#PBS -N senario.tcp

## Output and error files
#PBS -o senario.tcp.out
#PBS -e senario.tcp.err

## Limit memory, runtime etc.
#PBS -l walltime=04:00:00

## How many nodes:processors_per_node should we get?
#PBS -l nodes=8:ppn=8:mx10g:lowmem

## Start 
##echo "PBS_NODEFILE = $PBS_NODEFILE"
##cat $PBS_NODEFILE

## Run the job (use full paths to make sure we execute the correct thing) 
for size in 1024 2048 4096 6144
do
   echo /home/users/goumas/benchmarks/heat2_serial $size $size
   echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 2  --bynode /home/users/goumas/benchmarks/heat2_MPI $size $size 2 1 
   echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 4  --bynode /home/users/goumas/benchmarks/heat2_MPI $size $size 2 2 
   echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 8  --bynode /home/users/goumas/benchmarks/heat2_MPI $size $size 4 2 
   echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 16 --bynode /home/users/goumas/benchmarks/heat2_MPI $size $size 4 4 
   echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI $size $size 8 4 
   echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 64 --bynode /home/users/goumas/benchmarks/heat2_MPI $size $size 8 8 
done

echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI 4096 4096 32 1 
echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI 4096 4096 16 2 
echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI 4096 4096 8  4 
echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI 4096 4096 4  8 
echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI 4096 4096 2  16 
echo /usr/local/open-mpi/bin/mpirun -x MX_RCACHE=8 --mca btl tcp,self -np 32 --bynode /home/users/goumas/benchmarks/heat2_MPI 4096 4096 1  32 

for size in 1024 2048 4096 6144
do
   for threads in $(seq 1 8) 
   do
      echo /home/users/goumas/benchmarks/heat2_omp $size $size $threads
   done
done
