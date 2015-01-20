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

keys = {"sdl": 'Seidel SOR', "rbl": 'Red-Black SOR', "tjb": 'Tiled Jacobi'}
scaling = []

# anonymous normalization function to find speedup
normalize = lambda performance: [performance[0] / i for i in performance]

for size in [512, 1024, 2048]:
    with open('scl{}{}.dat'.format(mkey, size), 'r') as f:
        scaling.append(normalize([float(i.split()[0]) for i in f]))

ind = np.arange(len(scaling[0]))
fig, ax = plt.subplots()

# plot all lines with different colors
plt.plot(ind + 1, scaling[0], linestyle='-', marker='v', color='r', label='512')
plt.plot(ind + 1, scaling[1], linestyle='-', marker='v', color='b', label='1024')
plt.plot(ind + 1, scaling[2], linestyle='-', marker='v', color='y', label='2048')

# set titles and legend
plt.xlabel('Threads')
plt.ylabel('Speedup')
plt.legend()
plt.title("Speedup - {}".format(keys[mkey]))

# save figure
plt.savefig('scale{}.png'.format(mkey))

