#!/bin/bash

## Parallel systemsLab 2012-13
## To run on queue clones:
## Connect to scirouter.cslab.ece.ntua.gr and type
## qsub -q clones run_on_clones.sh

## Give the Job a descriptive name
#PBS -N par-lab-ask1 

## Output and error files
#PBS -o run.out
#PBS -e run.err

## Limit memory, runtime etc.
##PBS -l walltime=01:00:00
##PBS -l pmem=1gb

## How many nodes:processors_per_node should we get?
#PBS -l nodes=8:ppn=8

## Start
## Run the job (use full paths to make sure we execute the correct things
## Just replace the paths with your local paths to each file
serial_exe=/path/to/executable/serial_exe
continuous_collective_exe=/path/to/executable/continuous_collective_exe
continuous_p2p_exe=/path/to/executable/continuous_p2p_exe
round_robin_collective_exe=/path/to/executable/round_robin_collective_exe
round_robin_p2p_exe=/path/to/executable/round_robin_p2p_exe

for N in 2048 4096 6144
do
	## Execute Serial
	$serial_exe $N

	## Execute Parallels
	for p in 2 4 8 16 32 64; do
		/usr/local/open-mpi/bin/mpirun --mca orte_tmpdir_base /dev/shm -np $p --bynode \
									$continuous_collective_exe $N
		/usr/local/open-mpi/bin/mpirun --mca orte_tmpdir_base /dev/shm -np $p --bynode \
									$continuous_p2p_exe $N
		/usr/local/open-mpi/bin/mpirun --mca orte_tmpdir_base /dev/shm -np $p --bynode \
									$round_robin_collective_exe $N
		/usr/local/open-mpi/bin/mpirun --mca orte_tmpdir_base /dev/shm -np $p --bynode \
									$round_robin_p2p_exe $N
	done
done 
