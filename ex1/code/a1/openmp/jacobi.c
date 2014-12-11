#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"

int main(int argc, char ** argv)
{
	int global[2], local[2];  // global/local matrix dim
	int global_padded[2];	  // dimensions if padding is needed

	int grid[2];	      	// processors per dimension

	struct timeval tts, ttf, tcs, tcf; // total and computation timers

	double **U; 
	double ** u_current, ** u_previous; // matrices for iterations

	/* TODO: Code is to be filled here */

	
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

	// allocate global matrix, initialize random values on boundary
	// and zero values on interior
	U = allocate2d(global_padded[0], global_padded[1]);
	init2d(U, global[0], global[1]); //possible bug? (sizes differ)

	// allocate u_current and u_previous

	printf("Global Padded: %d %d\n", global_padded[0], global_padded[1]);
	u_previous = U;
	u_current = allocate2d(global_padded[0], global_padded[1]);

	omp_set_num_threads(grid[0] * grid[1]);
	int nthreads = grid[0] * grid[1];
	#pragma omp parallel 
	{
		// calculate block limits
		int tid = omp_get_thread_num();
		int i, j; 

		i = (global_padded[0] / grid[0]) * (tid % grid[0]);
		j = (global_padded[1] / grid[1]) * (tid / grid[0]);

		#ifdef DEBUG
			printf("Thread %d/%d gets (%d, %d) indices\n", tid, nthreads, i, j);
		#endif
	}

	return 0;
}
