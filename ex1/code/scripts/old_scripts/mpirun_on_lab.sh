#!/bin/bash

## Give the Job a descriptive name
#PBS -N testjob

## Output and error files
#PBS -o testjob.out
#PBS -e testjob.err

## Limit memory, runtime etc.
#PBS -l walltime=01:00:00
#PBS -l pmem=100mb

## How many nodes should we get?
#PBS -l nodes=8

## Start 
#echo "PBS_NODEFILE = $PBS_NODEFILE"
#cat $PBS_NODEFILE

## Run the job (use full paths to make sure we execute the correct thing) 
mpirun -np 8 -hostfile $PBS_NODEFILE --bynode /home/parallel/parlab40/fw/fw_MPI 2048

