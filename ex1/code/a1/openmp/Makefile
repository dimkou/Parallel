CC=gcc
CFLAGS=-O3 -g -Wall -fopenmp
MATH=-lm

all: jacobi redblacksor seidelsor jacobi_tiled

scaled: jacobi_tiled_s redblacksor_s seidelsor_s

redblacksor_s: redblacksor_scaled.c utils.c
	$(CC) $(CFLAGS) redblacksor_scaled.c utils.c -o redblacksor_s $(MATH)

seidelsor_s: seidelsor_scaled.c utils.c
	$(CC) $(CFLAGS) seidelsor_scaled.c utils.c -o seidelsor_s $(MATH)

jacobi_tiled_s: jacobi_tiled_scaled.c utils.c
	$(CC) $(CFLAGS) jacobi_tiled_scaled.c utils.c -o jacobi_tiled_s
	

jacobi: jacobi.c utils.c 
	$(CC) $(CFLAGS) jacobi.c utils.c -o jacobi

redblacksor: redblacksor.c utils.c
	$(CC) $(CFLAGS) redblacksor.c utils.c -o redblacksor $(MATH)

seidelsor: seidelsor.c utils.c
	$(CC) $(CFLAGS) seidelsor.c utils.c -o seidelsor $(MATH)

jacobi_tiled: jacobi_tiled.c utils.c
	$(CC) $(CFLAGS) jacobi_tiled.c utils.c -o jacobi_tiled
	
clean:
	rm jacobi redblacksor seidelsor jacobi_tiled jacobi_tiled_s seidelsor_s redblacksor_s

