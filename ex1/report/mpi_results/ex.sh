#!/usr/bin/env bash

rm *.dat *.png 2>/dev/null

for algo in jacobi seidel redblack
do
	./sortcores.py $algo
done

for size in 2048 4096 6144
do
	./plots.py $size
done

for algo in jacobi seidel redblack
do
	./speedup.py $algo
done

