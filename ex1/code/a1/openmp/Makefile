CC=gcc
CFLAGS=-O3 -lm -Wall -fopenmp

all: jacobi

jacobi: jacobi.c utils.c 
	$(CC) $(CFLAGS) $(RES) jacobi.c utils.c -o jacobi

clean:
	rm jacobi

