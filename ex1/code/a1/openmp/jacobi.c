#include <stdio.h>
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
	int global[2], local[2];  // global/local matrix dim
	int global_padded[2];	  // dimensions if padding is needed

	int grid[2];	      	// processors per dimension

	// struct timeval tts, ttf, tcs, tcf; // total and computation timers(TODO: use them!)

	double ** u_current, ** u_previous; // matrices for iterations


	
	if (argc != 5)
	{
		fprintf(stderr, "Usage: ./execname X Y Px Py\n");
		// Px and Py can be utilized via the omp_set_num_threads() routine
		exit(-1);
	}
	else {
		global[0] = atoi(argv[1]);
		global[1] = atoi(argv[2]);
		grid[0] = atoi(argv[3]);
		grid[1] = atoi(argv[4]);
	}
	
	int i; 
	/* apply padding, if necessary, to match
	 * dimensions to processor number */
	for (i = 0; i < 2; i++) {
        if (global[i] % grid[i]==0) {
            local[i]= global[i] / grid[i];
            global_padded[i] = global[i];
        }
        else 
		{
            local[i] = (global[i] / grid[i]) + 1;
            global_padded[i] = local[i] * grid[i];
        }
    }

	// allocate initial matrix, initialize random values on boundary
	// and zero values on interior
	u_previous = allocate2d(global_padded[0], global_padded[1]);
	init2d(u_previous, global[0], global[1]); //possible bug? (sizes differ)

	// allocate u_current
	u_current = allocate2d(global_padded[0], global_padded[1]);

	// transfer boundary values
	copy2d(u_current, u_previous, global[0], global[1]);

	// set number of threads
	int nthreads = grid[0] * grid[1];
	omp_set_num_threads(nthreads);

	// convergence flags
	int converged = 0;
	int globalConv = 0;

	#pragma omp parallel
	{
		// calculate block limits
		int tid = omp_get_thread_num();
		int i, j, endi, endj, conv;

		i = (global_padded[0] / grid[0]) * (tid % grid[0]);
		j = (global_padded[1] / grid[1]) * (tid / grid[0]);

		// calculate index limits. Locations on the edge of the matrix are not calculated
		endi = ((i + local[0]) < global[0]) ? i + local[0] : (global[0] - 1);
		endj = ((j + local[1]) < global[1]) ? j + local[1] : (global[1] - 1);
		
		// fix start limits for edges of matrix
		i += (i == 0);
		j += (j == 0);

		#ifdef DEBUG
			printf("Thread %d/%d gets (%d-%d, %d-%d) indices\n", tid, nthreads, i, endi, j, endj);
		#endif
		
				
		while (!globalConv) {
			jacobi(u_previous, u_current, i, endi, j, endj);
			conv = converge(u_previous, u_current, i, endi, j, endj);
			
			#pragma omp atomic
			converged += conv;

			#pragma omp barrier
			if (tid == 0) {
				if (converged == nthreads) { globalConv = 1; }
				else { converged = 0; }
				if (! globalConv) { copy2d(u_previous, u_current, global[0], global[1]); }
			}
			
			#pragma omp barrier
	
		}

	}
	fprint2d("test.out", u_current, global[0], global[1]);
	return 0;
}
