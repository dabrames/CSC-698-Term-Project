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

void do_block_fast (int lda, int M, int N, int K, double* A, double* B, double* C, int block_size)
{
  double a1, a2, a3, a4,
         b1, b2, b3, b4,
         c1, c2, c3, c4 = 0;

  double a[block_size*block_size] __attribute__ ((aligned (16)));
  // make a local aligned copy of A's block
  for( int i = 0; i < M; i++ )
    for( int j = 0; j < K; j++ )
      a[j+i*block_size] = A[i+j*lda];
  /* For each row i of A */
  for (int i = 0; i < M; ++i)
    
    /* For each column j of B */
    for (int j = 0; j < N; ++j) {
      /* Compute C(i,j) */
      double cij = C[i+j*lda];
      
      for (int k = 0; k < K; k += 4) {
        a1 = a[i+k*block_size];
        a2 = a[i+(k+1)*block_size];
        a3 = a[i+(k+2)*block_size];
        a4 = a[i+(k+3)*block_size];
        
        b1 = B[k+j*lda];;
        b2 = B[(k+1)+j*lda];
        b3 = B[(k+2)+j*lda];
        b4 = B[(k+3)+j*lda];
        
        c1 = a1 * b1;
        c2 = a2 * b2;
        c3 = a3 * b3;
        c4 = a4 * b4;
 
        cij += c1;
        cij += c2;
        cij += c3;
        cij += c4;
    }

    C[i+j*lda] = cij;
  }
}

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format. 
 * On exit, A and B maintain their input values. */  
void square_dgemm_blocked_unrolled(int lda, double* A, double* B, double* C, int block_size)
{
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
          do_block_fast(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda, block_size);
        } else {
          /* Perform individual block dgemm */
	        do_block(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda, block_size);
        }
      }
}