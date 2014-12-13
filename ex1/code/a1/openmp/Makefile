CC=gcc
CFLAGS=-O3 -Wall -fopenmp
DFLAGS=-DDEBUG
MATH=-lm

all: jacobi redblacksor

jacobi: jacobi.c utils.c 
	$(CC) $(CFLAGS) $(RES) jacobi.c utils.c -o jacobi

redblacksor: redblacksor.c utils.c
	$(CC) $(CFLAGS) $(DFLAGS) redblacksor.c utils.c -o redblacksor $(MATH)

clean:
	rm jacobi redblacksor

