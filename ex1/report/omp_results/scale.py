#!/usr/bin/python

import sys
from collections import defaultdict

def computeTimes(algorithm_performance):

    tuple_add = lambda x, y: (x[0] + y[0], x[1] + y[1])

    perform = {}
    for thread in range(1, 9):
        # get performance for certain number of threads
        tempData = [i[1:3] for i in algorithm_performance if i[0] == thread]
        perform[thread] = tuple(map(lambda x: x / 3, reduce(tuple_add, tempData)))

    return perform

filenames = ['ompscale512.out',
             'ompscale1024.out',
             'ompscale2048.out']


try:
    fil = 'ompscale{}.out'.format(sys.argv[1])
    matrix_size = int(sys.argv[1])
except IndexError:
    sys.stderr.write("Usage: ./scale.py size\n")
    exit(0)


data = [i.split() for i in open(fil, 'r')]
thread_data = defaultdict(list)

for line in data:
    try:
        thread_data[line[0]].append((int(line[2]), float(line[4]), float(line[6])))
    except ValueError:
        sys.stderr.write("Output file format is wrong!\n")
        exit(0)
    

for key in ['sdl', 'rbl', 'tjb']:

    with open('scl{}{}.dat'.format(key, matrix_size), 'w') as f:

        performance = computeTimes(thread_data[key])
        
        for v in performance.values():
            f.write('{} {}\n'.format(v[0], v[1]))
                

