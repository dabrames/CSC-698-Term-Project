
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


double* matA;
double* matB;
double* matC;

int nthreads = 1;
int maxsize = 0;

#define pth_num 4

int gM;
int gN;
int gK;
double *gA;
double *gB;
double *gC;

/* 
    Please include compiler name below (you may also include any other modules you would like to be loaded)
COMPILER= Intel icc
    Please include All compiler flags and libraries as you want them run. You can simply copy this over from the Makefile's first few lines
 
CC = icc
OPT = -O3
CFLAGS = -Wall -std=gnu99 $(OPT)
MKLROOT = /opt/intel/composer_xe_2013.1.117/mkl
LDLIBS = -lrt -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm
*/

#define min(a,b) (((a)<(b))?(a):(b))


void* worker (void* arg){
	
	int tid = *(int *)(arg); // get the thread ID assigned sequentially.
	int start = tid * (gN / nthreads);
	int end = (tid + 1) * (gN / nthreads);

	//printf("Worker thread %d",tid);

	for (int i = start; i < end; i++) 
        for (int j = 0; j < gN; j++){ 
        	double cij = matC[i+j*gN];
            for (int k = 0; k < gK; k++) 
                cij += matA[i + k*gN] * matB[k + j*gN];
            matC[i+j*gN] = cij;
        }

}

/* This auxiliary subroutine performs a smaller dgemm operation
 *  C := C + A * B
 * where C is M-by-N, A is M-by-K, and B is K-by-N. */
static void do_block (int lda, int M, int N, int K, double* A, double* B, double* C, int block_size)
{
  int mul_j_lda;
  int res_i;
  int prod_k_lda;
  //static unsigned int
  /* For each row i of A */
  for (int i = 0; i < M; ++i){
    /* For each column j of B */
    for (int j = 0; j < N; ++j)
    {
      /* Compute C(i,j) */
      mul_j_lda = j * lda;
      res_i = i + mul_j_lda;
      double cij = C[res_i];

      for (int k = 0; k < K; ++k){
        prod_k_lda = k * lda;
        cij += A[i + prod_k_lda] * B[k + mul_j_lda];
      }
      C[i + mul_j_lda] = cij;
    }
  }
}

void do_block_fast_pth ()
{

	pthread_t worker_threads[pth_num];

	//printf("Creating pthtreads\n");
	//Creating pthreads and starting them
	for(int i = 0; i < pth_num; i++){
		int *tid;
		tid = (int *) malloc(sizeof(int));
		*tid = i;
		//printf("Phread with id = %d", *tid);
		pthread_create(&worker_threads[i], NULL, worker, (void *) tid);
	}

	//printf("Joining pthreads\n");
	for(int i=0; i < pth_num; i++){
		pthread_join(worker_threads[i], NULL);
	}
}




/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format. 
 * On exit, A and B maintain their input values. */  
void square_dgemm_blocked_unrolled_pthreads(int lda, double* A, double* B, double* C, int block_size)
{

	matA = A;
	matB = B;
	matC = C;
	maxsize = lda;

  /* For each block-row of A */ 
  for (int i = 0; i < lda; i += block_size)
    /* For each block-column of B */
    for (int j = 0; j < lda; j += block_size)
      /* Accumulate block dgemms into block of C */
      for (int k = 0; k < lda; k += block_size) {
        /* Correct block dimensions if block "goes off edge of" the matrix */
	      int M = min (block_size, lda-i);
	      int N = min (block_size, lda-j);
	      int K = min (block_size, lda-k);

	      if ((M % block_size ==0) && (N % block_size ==0) && (K % block_size == 0)) {
          /* Perform square block dgemm */
          gM = M; gN = N; gK = k; gA = A + i + k*lda; gB = B + k + j*lda; gC = C + i + j*lda;
          
          do_block_fast_pth();			//lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda, block_size);
        
        } else {
          /* Perform individual block dgemm */
	       do_block(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda, block_size);
        }
      }
}




