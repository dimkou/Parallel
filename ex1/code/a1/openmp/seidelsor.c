#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"
#include <math.h>


void gauss(double ** u_previous, double ** u_current, int start_x, 
		 int start_y, int range, int dimX, int dimY, double omega) 
{
	// computational core here
	// called from within threads
	int ct = 0,  i = start_x,  j = start_y;
	while (ct < range)
	{
				if ((i > 0) && (i < dimX - 1) && (j > 0) && (j < dimY - 1)) {
					u_current[i][j] = u_previous[i][j] + (omega / 4.0) * (u_current[i-1][j] + u_current[i][j-1] + 
									  u_previous[i+1][j] + u_previous[i][j+1] - 4 * u_previous[i][j]);
				}
				else 
					break;
				// iteration is done diagonally
				// for <range> times
                else {
                    break;
                }
				++i; --j; ++ct;
	}
}

int main(int argc, char ** argv)
{
	int global[2];  // global/local matrix dim
	int grid;	      	// processors for wavefront expansion
	struct timeval tts, ttf; // total and computation timers(TODO: use them!)

	double ** u_current, ** u_previous, ** swap; // matrices for iterations
	double omega;
		
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
	
	omega = 2.0 / (1 + sin(3.14 / global[0]));
	
	// allocate initial matrix, initialize random values on boundary
	// and zero values on interior
	u_previous = allocate2d(global[0], global[1]);
	init2d(u_previous, global[0], global[1]); //possible bug? (sizes differ)

	// allocate u_current
	u_current = allocate2d(global[0], global[1]);

	// transfer boundary values
	copy2d(u_current, u_previous, global[0], global[1]);

	// set number of threads
	int nthreads = grid;
	omp_set_num_threads(nthreads);

	// convergence flags
	int globalConv = 0;
    int iters = 0;

    double time = 0, compTime = 0;
    double * comps = calloc(nthreads, sizeof(double));

	#pragma omp parallel 
	{
		int tid = omp_get_thread_num();
		int local_i, local_j, range;
		int x, y, wavefront;
        struct timeval tcs, tcf;
    
        #ifndef SCALE
		while (!globalConv) {
        #endif
        #ifdef SCALE
        while (iters < 256) {
        #endif
            #pragma omp single
            {
                ++iters;
                gettimeofday(&tts, NULL);
            }

			x = 1; y = 1;
			while (x < global[0] - 1)
			{
				// side columns are constant
				x += (x == 0);

				while (y < global[1] - 1) {
			
					// top and bottom rows are constant
					y += (y == 0);

					wavefront = y + 1 - x;

					range = (wavefront / nthreads) + ((wavefront % nthreads) != 0);
					local_i = tid * range + x;
					local_j = y - tid * range;


					#ifdef DEBUG
						printf("%d gets (%d, %d) indices with range %d\n", tid, local_i, local_j, range);
					#endif

					#pragma omp barrier
				
                    
                    gettimeofday(&tcs, NULL);
					gauss(u_previous, u_current, local_i, local_j, range, global[0], global[1], omega);
                    gettimeofday(&tcf, NULL);
                    comps[tid] += (tcf.tv_sec - tcs.tv_sec) + (tcf.tv_usec - tcs.tv_usec) * 0.000001;
					
					++y;
					// synchronize before next loop for correct clock ticking
					#pragma omp barrier

				}
				
				wavefront = y - x;
				range = (wavefront / nthreads) + ((wavefront % nthreads) != 0);
				local_i = tid * range + x;
				local_j = (y - 1)  - tid * range;
					
				#ifdef DEBUG
					printf("%d gets (%d, %d) indices with range %d\n", tid, local_i, local_j, range);
				#endif

				#pragma omp barrier
			
                gettimeofday(&tcs, NULL);
				gauss(u_previous, u_current, local_i, local_j, range, global[0], global[1], omega);
                gettimeofday(&tcf, NULL);
                comps[tid] += (tcf.tv_sec - tcs.tv_sec) + (tcf.tv_usec - tcs.tv_usec) * 0.000001;
				
				++x;
				// synchronize before next loop for correct clock ticking
						
			}
			//check local convergence (no need for barrier here!)


			#pragma omp barrier
			
            #ifndef SCALE
			if ((iters % C == 0) && (tid == 0)) {
				if (converge(u_previous, u_current, 1, global[1]-1, 1, global[0]-1)) { globalConv = 1; }
			}
            #endif

			#pragma omp single
            {
                swap = u_previous; u_previous = u_current; u_current = swap; 
                gettimeofday(&ttf, NULL);
                time += (ttf.tv_sec - tts.tv_sec) + (ttf.tv_usec - tts.tv_usec) * 0.000001;
            }

		}
	}

    int i;
    for (i = 0; i < nthreads; ++i)
        compTime = (compTime >= comps[i]) ? compTime : comps[i];

	printf("SeidelSOR\tIters: %d Time: %.3lf \t Computation: %.3lf\n", iters, time, compTime);
	fprint2d("testseidel.out", u_current, global[0], global[1]);
	return 0;
}
