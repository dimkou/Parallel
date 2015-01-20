#!/usr/bin/env python

import sys
import numpy as np
import matplotlib.pyplot as plt

from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['KerkisSans']})

try:
    mkey = sys.argv[1]
except IndexError:
    sys.stderr.write("Usage: ./plots.py algorithm\n")
    exit(0)

keys = {"seidel": 'Seidel SOR', "redblack": 'Red-Black SOR', "jacobi": 'Jacobi'}
scaling = []

# anonymous normalization function to find speedup
normalize = lambda performance: [performance[0] / i for i in performance]

for size in [2048, 4096, 6144]:
    with open('scl{}{}.dat'.format(mkey, size), 'r') as f:
        scaling.append(normalize([float(i.split()[0]) for i in f]))

ind = np.arange(len(scaling[0]))
cores = 2 ** ind

fig, ax = plt.subplots()

# plot all lines with different colors
plt.plot(ind + 1, scaling[0], linestyle='-', marker='v', color='r', label='2048')
plt.plot(ind + 1, scaling[1], linestyle='-', marker='v', color='b', label='4096')
plt.plot(ind + 1, scaling[2], linestyle='-', marker='v', color='y', label='6144')

# set titles and legend
plt.xlabel('Cores')
plt.ylabel('Speedup')
plt.legend(loc=2)
plt.xticks(ind + 1, cores) 
plt.title("Speedup - {}".format(keys[mkey]))

# save figure
plt.savefig('scale{}.png'.format(mkey))

