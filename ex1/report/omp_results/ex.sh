#!/usr/bin/env bash

# remove .dat and picture files
rm *.dat *.png >/dev/null

for size in 512 1024 2048
do
	./scale.py $size
	./plots.py $size
done


