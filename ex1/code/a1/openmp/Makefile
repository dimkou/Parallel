CC=gcc
CFLAGS=-O3 -g -Wall -fopenmp
DFLAGS=-DDEBUG
MATH=-lm

all: jacobi redblacksor seidelsor jacobi_tiled

jacobi: jacobi.c utils.c 
	$(CC) $(CFLAGS) jacobi.c utils.c -o jacobi

redblacksor: redblacksor.c utils.c
	$(CC) $(CFLAGS) redblacksor.c utils.c -o redblacksor $(MATH)

seidelsor: seidelsor.c utils.c
	$(CC) $(CFLAGS) seidelsor.c utils.c -o seidelsor $(MATH)

jacobi_tiled: jacobi_tiled.c utils.c
	$(CC) $(CFLAGS) jacobi_tiled.c utils.c -o jacobi_tiled
	
clean:
	rm jacobi redblacksor seidelsor jacobi_tiled test.out

