CC=gcc
CFLAGS=-O3 -g -Wall -fopenmp
DFLAGS=-DDEBUG
MATH=-lm

all: jacobi redblacksor seidelsor

jacobi: jacobi.c utils.c 
	$(CC) $(CFLAGS) $(RES) jacobi.c utils.c -o jacobi

redblacksor: redblacksor.c utils.c
	$(CC) $(CFLAGS) redblacksor.c utils.c -o redblacksor $(MATH)

seidelsor: seidelsor.c utils.c
	$(CC) $(CFLAGS) seidelsor.c utils.c -o seidelsor $(MATH)

clean:
	rm jacobi redblacksor seidelsor

