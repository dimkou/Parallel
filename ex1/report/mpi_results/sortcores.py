#!/usr/bin/env python

import sys 
from collections import defaultdict

def compute_time(algorithm_performance):

    tuple_add = lambda x, y: (x[0] + y[0], x[1] + y[1])

    perform = {}
    for core in algorithm_performance.keys():
        perform[core] = tuple(map(lambda x: x / 3, reduce(tuple_add, algorithm_performance[core])))

    return perform


try:
    algo = sys.argv[1]
except IndexError:
    sys.stderr.write("Usage: ./sortcores.py [jacobi|seidel|redblack]\n")
    exit(0)

# never accept wrong input!
if algo not in ['jacobi', 'seidel', 'redblack']:
    sys.stderr.write("Wrong algorithm specified! Use one of 'jacobi', 'seidel', 'redblack'.\n")
    exit(0)

# lines kept should contain: #cores, total_time, comp_time
linefilter = lambda dataline: (int(dataline[6]) * int(dataline[8]), float(dataline[14]), float(dataline[12])) 

for size in [2048, 4096, 6144]:
    
    thread_data = defaultdict(list)

    with open('{}_{}'.format(algo, size), 'r') as f:
        lines = map(linefilter, (i.split() for i in f))

    # retrieve per-thread data
    for elem in lines: 
        thread_data[elem[0]].append((elem[1], elem[2])) 
    
    # compute means for performance measurement
    performance = compute_time(thread_data)

    # output performance-per-thread to .dat file
    with open('scl{}{}.dat'.format(algo, size), 'w') as df:

        for item in sorted(performance):
            df.write(' '.join(map(str, performance[item])) + '\n')








