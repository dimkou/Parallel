CC=gcc
CFLAGS=-O3 -lm -Wall
RES=-DPRINT_RESULTS
CONV=-DTEST_CONV

aall: jacobi seidelsor redblacksor


jacobi: Jacobi_naive.c utils.c
	$(CC) $(CFLAGS) $(RES) $(CONV) Jacobi_naive.c utils.c -o jacobi

seidelsor: GaussSeidelSOR_naive.c utils.c
	$(CC) $(CFLAGS) $(RES) $(CONV) GaussSeidelSOR_naive.c utils.c -o seidelsor

redblacksor: RedBlackSOR_naive.c utils.c 
	$(CC) $(CFLAGS) $(RES) $(CONV) RedBlackSOR_naive.c utils.c -o redblacksor

clean:
	rm jacobi seidelsor redblacksor

