#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"

void jacobi(double ** u_previous, double ** u_current, int min_x, int max_x, int min_y, int max_y) 
{
	// computational core here
	// called from within threads
	int i, j;
	for (i = min_x; i < max_x; i++)
		for (j = min_y; j < max_y; j++)
			u_current[i][j] = (u_previous[i-1][j] + u_previous[i][j-1] + u_previous[i+1][j] + u_previous[i][j+1]) / 4.0;
}


int main(int argc, char ** argv)
{
	int global[2], local;  // global/local matrix dim
	int global_padded[2];	  // dimensions if padding is needed

	int grid;	  		      	// processors per dimension
	struct timeval tts, ttf, tcs, tcf; // total and computation timers(TODO: use them!)

	double ** u_current, ** u_previous; // matrices for iterations
	double ** swap;

	
	if (argc != 4)
	{
		fprintf(stderr, "Usage: ./execname X Y P\n");
		exit(-1);
	}
	else {
		global[0] = atoi(argv[1]);
		global[1] = atoi(argv[2]);
		grid = atoi(argv[3]);
	}
	
	int i; 
	/* apply padding, if necessary, to match
	 * dimension to processor number */
	if (global[0] % grid == 0) {
            local = global[0] / grid;
            global_padded[0] = global[0];
			global_padded[1] = global[1];
	}
    else 
	{
            local = (global[0] / grid) + 1;
            global_padded[0] = local * grid;
			global_padded[1] = global[1];
	}
	// since we are using loop tiling, we only need 
	// to consider padding on x-dimension
	


	// allocate initial matrix, initialize random values on boundary
	// and zero values on interior
	u_previous = allocate2d(global_padded[0], global_padded[1]);
	init2d(u_previous, global[0], global[1]); //possible bug? (sizes differ)

	// allocate u_current
	u_current = allocate2d(global_padded[0], global_padded[1]);

	// transfer boundary values
	copy2d(u_current, u_previous, global[0], global[1]);

	// set number of threads
	int nthreads = grid;
	omp_set_num_threads(nthreads);

	// convergence flags
	int converged = 1;
	int globalConv = 0; int iter = 1;
	#pragma omp parallel
	{
		// calculate block limits
		int tid = omp_get_thread_num();
		int i = (global_padded[0] / grid) * (tid % grid);
		int endi = ( (i + local) < global[0]) ? i + local : global[0] - 1;
		int j, conv;
		conv = 1;
		// fix start limits for edges of matrix
		i += (i == 0);

		#ifdef DEBUG
			printf("Thread %d gets (%d-%d) indices with range %d\n", tid, i, endi, local);
		#endif
		
				
		while (!globalConv) {

			gettimeofday(&tcs, NULL);
			for (j = 1; j < global[1] - 1; j++) {
				jacobi(u_previous, u_current, i, endi, j, j+1);
			}
			gettimeofday(&tcf, NULL);

			if ((iter % C) == 0) {
				conv = 1;
				for (j = 1; j < global[1] ; j++) {
					conv &= converge(u_previous, u_current, i, endi-1, j, j);	
				}
				#pragma omp atomic
				converged += conv;
			}

			#pragma omp barrier

			if ((iter % C == 0) && (tid == 0)) {
				if (converged == nthreads) { globalConv = 1; printf("Globalconv! %d\n", globalConv); }
			}
			
			#pragma omp single
			{
                converged = 0;
				swap = u_previous; u_previous = u_current; u_current = swap;
				++iter;
			}

	
		}
	}
    printf("Iters: %d\n", iter);
	fprint2d("test.out", u_current, global[0], global[1]);
	return 0;
}
