#!/usr/bin/env bash

# remove .dat and picture files
rm *.dat *.png 2>/dev/null

for size in 512 1024 2048
do
	./scale.py $size
	./plots.py $size
done

for algo in rbl sdl tjb
do
	./speedup.py $algo
done


