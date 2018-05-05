#include <omp.h>


/*
* Used OpenMPI to parallelize the Matrix Multiplication in naive implementation.
* @param: dimension - Size of input and output array. Should ne square matrix
* 		  double* A, B, C : A, B input matrix and C outout matrix
		  int threads : Number of threads OpenMP should launch as kernel.
*/
void square_dgemm_naive_openmp(int dimension, double* matrixA, double* matrixB, double* matrixC, int theads){

	int i, j, k, iOff, jOff;
	double tot;

	#pragma omp parallel shared(matrixC) private(i, j, k, iOff, jOff, tot) num_threads(theads)
	{
		#pragma omp for schedule(static)
		for(i=0; i<dimension; i++){
			iOff = i * dimension;
			for(j=0; j<dimension; j++){
				jOff = j * dimension;
				tot = 0;
				for(k=0; k<dimension; k++){
					tot += matrixA[iOff + k] * matrixB[jOff + k];
				}
				matrixC[i][j] = tot;
			}
		}
	}
}