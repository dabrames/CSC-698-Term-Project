/* 
    Please include compiler name below (you may also include any other modules you would like to be loaded)
COMPILER= gnu
    Please include All compiler flags and libraries as you want them run. You can simply copy this over from the Makefile's first few lines
 
CC = cc
OPT = -O3
CFLAGS = -Wall -std=gnu99 $(OPT)
MKLROOT = /opt/intel/composer_xe_2013.1.117/mkl
LDLIBS = -lrt -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm
*/
#include "immintrin.h"

const char* dgemm_desc = "Simple blocked dgemm.";

#define min(a,b) (((a)<(b))?(a):(b))


/* This auxiliary subroutine performs a smaller dgemm operation
 *  C := C + A * B
 * where C is M-by-N, A is M-by-K, and B is K-by-N. */
static void do_block (int lda, int M, int N, int K, double* A, double* B, double* C)
{
  unsigned int mul_j_lda;
  unsigned int res_i;
  unsigned int prod_k_lda;
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

 static void do_block_fast(int lda, int M, int N, int K, double* A, double* B, double* C, int block_size)
 {
  
  unsigned int prod1 = 1;
  unsigned int prod2 = 1;
  unsigned int res1 = 0;
  unsigned int res2 = 0;

   double a[block_size * block_size] __attribute__((aligned (32)));
   double temp[4] __attribute__((aligned (32)));

  //SIMD variables defined

  __m128d vec128Ctmp1;
  __m128d vec128Ctmp2;

  __m256d vec1A;
  __m256d vec1B;
  __m256d vec1C;

  __m256d vec2A;
  __m256d vec2B;
  __m256d vec2C;

  __m256d vecCtmp;
  __m128d vecCtmp2;

  //  make a local aligned copy of A's block
  
  for( int j = 0; j < K; j++ ) {
    prod2 = j * lda;
    for( int i = 0; i < M; i++ )
    {
      prod1 = i * block_size;
      res1 = prod1 + j;
      res2 = prod2 + i;
      a[res1] = A[res2];
    }
  }

  /* For each row i of A */
    for (int i = 0; i < M; ++i){
      prod1 = i * block_size;
    /* For each column j of B */ 
      for (int j = 0; j < N; ++j) 
      {

      /* Compute C(i,j) */
        prod2 = j * lda;
        res2 = i + prod2;
        
        double cij = C[res2]; //C[i+j*lda];
        
        for (int k = 0; k < K; k = k + 8){
          //   cij += a[i+k*BLOCK_SIZE] * B[k+j*lda];  
          //prod2 = j*lda;
          res1 = k + prod1;           vec1A = _mm256_load_pd  (&a[res1]);       //k+(i*BLOCK_SIZE)
          res2 = k + prod2;           vec1B = _mm256_loadu_pd (&B[res2]);       //k+(j*lda)
          res1 = res1 + 4;            vec2A = _mm256_load_pd  (&a[res1]);       //(k+4)+i*BLOCK_SIZE   (k + 4) + prod1;
          res2 = res2 + 4;            vec2B = _mm256_loadu_pd (&B[res2]);       //(k+4)+j*lda   (k + 4) + prod2;
          
          vec1C = _mm256_mul_pd(vec1A, vec1B);
          vec2C = _mm256_mul_pd(vec2A, vec2B);
          
          vecCtmp = _mm256_add_pd(vec1C, vec2C);
          
          _mm256_store_pd(&temp[0], vecCtmp);
          
          //vec128Ctmp1 = _mm_load_pd(&temp[0]);
          //vec128Ctmp2 = _mm_load_pd(&temp[2]);

          //vecCtmp2 = _mm_add_pd(vec128Ctmp1, vec128Ctmp2);

          //_mm_storeu_pd(&temp[0], vecCtmp2);

          cij += temp[0];
          cij += temp[1];
          cij += temp[2];
          cij += temp[3];
        }

        res2 = i + prod2;
        C[res2] = cij;
        //C[i+j*lda] = cij;
      }
    }

  }

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format. 
 * On exit, A and B maintain their input values. */  
  void square_dgemm_blocked_unrolled_simd (int lda, double* A, double* B, double* C, int block_size)
  {
    int mul_j;
    int mul_k;
  /* For each block-row of A */ 
    for (int i = 0; i < lda; i += block_size)
    /* For each block-column of B */
      for (int j = 0; j < lda; j += block_size){

        mul_j = j * lda;

      /* Accumulate block dgemms into block of C */
        for (int k = 0; k < lda; k += block_size)
        {
  /* Correct block dimensions if block "goes off edge of" the matrix */
         int M = min (block_size, lda-i);
         int N = min (block_size, lda-j);
         int K = min (block_size, lda-k);

         mul_k = k * lda;

         double* res_A = A + (i + mul_k);
         double* res_C = C + (i + mul_j);
         double* res_B = B + (k + mul_j);

  /* Perform individual block dgemm */
        if((M == block_size) && (N == block_size) && (K == block_size)){
            //do_block_fast(lda, M, N, K, A + i + k*lda, B + k + j*lda, C + i + j*lda);
          do_block_fast(lda, M, N, K, res_A, res_B, res_C, block_size);
        }else{
    /* Perform individual block dgemm */
          do_block(lda, M, N, K, res_A, res_B, res_C);
        }
      }
    }
  }