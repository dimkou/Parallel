#!/usr/bin/env python

import sys
import numpy as np
import matplotlib.pyplot as plt

from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['KerkisSans']})

try:
    msize = sys.argv[1]
except IndexError:
    sys.stderr.write("Usage: ./plots.py size\n")
    exit(0)

keys = {"seidel": 'Seidel SOR', "redblack": 'Red-Black SOR', "jacobi": 'Jacobi'}

for key in keys:

    with open('scl{0}{1}.dat'.format(key, msize), 'r') as f:

        vals = [tuple(map(lambda x: float(x), i.split())) for i in f]
        ind = np.arange(len(vals))
        cores = 2 ** ind
        width = 0.4

        # if you don't want bars to overlap, set displ = width 
        displ = width 

        fig, ax = plt.subplots()
        
        bf_1 = ax.bar(ind + 1, [i[0] for i in vals], width, color='r', label='Total')
        bf_2 = ax.bar(ind + 1 + displ, [i[1] for i in vals], width, color='y', label='Computation')

        # set titles and graphics
        plt.xlabel('Cores')
        plt.ylabel('Time')
        plt.title('{0} - {1}x{1}'.format(keys[key], msize))
        plt.legend()
        plt.xticks(ind + 1, cores)
        plt.savefig('bars{}{}.png'.format(key, msize), ext="png")

