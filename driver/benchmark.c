#include <stdlib.h> // For: exit, drand48, malloc, free, NULL, EXIT_FAILURE
#include <stdio.h>  // For: perror
#include <string.h> // For: memset

#include <float.h>  // For: DBL_EPSILON
#include <math.h>   // For: fabs
#include <pthread.h>
#include <sys/stat.h> // For creating output files directory

#ifdef GETTIMEOFDAY
#include <sys/time.h> // For struct timeval, gettimeofday
#else
#include <time.h> // For struct timespec, clock_gettime, CLOCK_MONOTONIC
#endif

/* Your function must have the following signature: */
extern void square_dgemm_blocked_unrolled (int, double*, double*, double*, int);
extern void square_dgemm_naive (int, double*, double*, double*);
extern void square_dgemm_blocked_naive (int, double*, double*, double*, int);
extern void square_dgemm_naive_pthreads(int, double*, double*, double*, int);
extern void square_dgemm_blocked_unrolled_simd(int, double*, double*, double*, int);

extern void square_dgemm_blocked_unrolled_pthreads(int, double*, double*, double*, int);

double wall_time ()
{
#ifdef GETTIMEOFDAY
    struct timeval t;
    gettimeofday (&t, NULL);
    return 1.*t.tv_sec + 1.e-6*t.tv_usec;
#else
    struct timespec t;
    clock_gettime (CLOCK_MONOTONIC, &t);
    return 1.*t.tv_sec + 1.e-9*t.tv_nsec;
#endif
}

void die (const char* message)
{
    perror (message);
    exit (EXIT_FAILURE);
}

void fill (double* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i] = 2 * drand48() - 1; // Uniformly distributed over [-1, 1]
}

int main() {
    //Benchmarking sample sizes
    //Make sure to have the sizes in increasing order
    int test_sizes[] = {31, 32, 96, 97, 127, 128, 129, 191, 192, 229, 255, 256, 257, 319, 320, 321, 417, 479, 480,
                        511, 512, 639, 640, 767, 768, 769};
    int nSizes = sizeof(test_sizes)/sizeof(test_sizes[0]);

    //Hoping that last size is also the largest size
    int nMax = test_sizes[nSizes-1];

    //Block sizes for algorithm using blocks
    int block_sizes[] = {16,32,64,128,256,512};
    int nBlocks = sizeof(block_sizes)/ sizeof(block_sizes[0]);

    //Thread sizes for algorithms using threas
    int pthread_sizes[] = {2,4,6,8,16,32,64};
    int npthreads = sizeof(pthread_sizes)/ sizeof(pthread_sizes[0]);

    //Allocating buffer for input and output matrices
    double* buf = NULL;
    buf = (double*) malloc (3 * nMax * nMax * sizeof(double));
    if (buf == NULL) die ("failed to allocate largest problem size");
    
    // Using iterations for averaging out the result
    int iterations = 1;
    double start_time, time;

    // Create output files
    struct stat st = {0};
    if (stat("../output-files/", &st) == -1) {
    	mkdir("../output-files/", 0700);
    }

    //Benchmarking naive version with different sample sizes
    for (int iSize = 0; iSize < nSizes; ++iSize)
    {
        int n = test_sizes[iSize];
        double* A = buf + 0;
        double* B = A + nMax*nMax;
        double* C = B + nMax*nMax;

        fill (A, n*n);
        fill (B, n*n);
        fill (C, n*n);

        time = 0;
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm_naive(n, A, B, C);
            time += wall_time() - start_time;
        }
        time = time / iterations;
        FILE *fp;
        printf("%i %f\n", n, time);
        fp = fopen("../output-files/naive.txt", "a");
        fprintf(fp, "%i %f\n", n, time);
        fclose(fp);
    }
    printf("Done with Naive version");

    //Benchmarking Blocked Naive version with different block sizes
    for(int iBlock = 0; iBlock < nBlocks; iBlock++){
        for (int iSize = 0; iSize < nSizes; ++iSize)
        {
            int n = test_sizes[iSize];
            double* A = buf + 0;
            double* B = A + nMax*nMax;
            double* C = B + nMax*nMax;

            fill (A, n*n);
            fill (B, n*n);
            fill (C, n*n);

            time = 0;
            for(int i = 0; i < iterations; i++){
                start_time = wall_time();
                square_dgemm_blocked_naive(n, A, B, C, block_sizes[iBlock]);
                time += wall_time() - start_time;
            }
            time = time / iterations;
            FILE *fp;
            printf("%i %f %i\n", n, time, block_sizes[iBlock]);
            fp = fopen("../output-files/blocked-naive.txt", "a");
            fprintf(fp, "%i %f %i\n", n, time, block_sizes[iBlock]);
            fclose(fp);
        }
    }
    printf("Done with Blocked Naive version");

    //Benchmarking Blocked Unrolled version with different block sizes
    for(int iBlock = 0; iBlock < nBlocks; iBlock++){
        for (int iSize = 0; iSize < nSizes; ++iSize)
        {
            int n = test_sizes[iSize];
            double* A = buf + 0;
            double* B = A + nMax*nMax;
            double* C = B + nMax*nMax;

            fill (A, n*n);
            fill (B, n*n);
            fill (C, n*n);

            time = 0;
            for(int i = 0; i < iterations; i++){
                start_time = wall_time();
                square_dgemm_blocked_unrolled(n, A, B, C, block_sizes[iBlock]);
                time += wall_time() - start_time;
            }
            time = time / iterations;
            FILE *fp;
            printf("%i %f %i\n", n, time, block_sizes[iBlock]);
            fp = fopen("../output-files/blocked_unrolled.txt", "a");
            fprintf(fp, "%i %f %i\n", n, time, block_sizes[iBlock]);
            fclose(fp);
        }
    }
    printf("Done with Blocked Unrolled version");

    //Benchmarking Blocked Unrolled Simd version with different block sizes
    for(int iBlock = 0; iBlock < nBlocks; iBlock++){
        for (int iSize = 0; iSize < nSizes; ++iSize)
        {
            int n = test_sizes[iSize];
            double* A = buf + 0;
            double* B = A + nMax*nMax;
            double* C = B + nMax*nMax;

            fill (A, n*n);
            fill (B, n*n);
            fill (C, n*n);

            time = 0;
            for(int i = 0; i < iterations; i++){
                start_time = wall_time();
                square_dgemm_blocked_unrolled_simd(n, A, B, C, block_sizes[iBlock]);
                time += wall_time() - start_time;
            }
            time = time / iterations;
            FILE *fp;
            printf("%i %f %i\n", n, time, block_sizes[iBlock]);
            fp = fopen("../output-files/blocked_unrolled_simd.txt", "a");
            fprintf(fp, "%i %f %i\n", n, time, block_sizes[iBlock]);
            fclose(fp);
        }
    }
    printf("Done with Blocked Unrolled Simd version");


    //Benchmarking Pthread version with different pthread numbers
    for(int ipthread = 0; ipthread < npthreads; ipthread++){
        for (int iSize = 0; iSize < nSizes; ++iSize)
        {
            int n = test_sizes[iSize];
            double* A = buf + 0;
            double* B = A + nMax*nMax;
            double* C = B + nMax*nMax;

            fill (A, n*n);
            fill (B, n*n);
            fill (C, n*n);

            time = 0;
            for(int i = 0; i < iterations; i++){
                start_time = wall_time();
                square_dgemm_naive_pthreads(n, A, B, C, pthread_sizes[ipthread]);
                time += wall_time() - start_time;
            }
            time = time / iterations;
            FILE *fp;
            printf("%i %f %i\n", n, time, pthread_sizes[ipthread]);
            fp = fopen("../output-files/naive_pthreads.txt", "a");
            fprintf(fp, "%i %f %i\n", n, time, pthread_sizes[ipthread]);
            fclose(fp);
        }
    }
    printf("Done with Pthread version");

    //Benchmarking Blocked Pthreads version with different block sizes    
    for(int iBlock = 0; iBlock < nBlocks; iBlock++){
        for (int iSize = 0; iSize < nSizes; ++iSize)
        {
            int n = test_sizes[iSize];
            double* A = buf + 0;
            double* B = A + nMax*nMax;
            double* C = B + nMax*nMax;

            fill (A, n*n);
            fill (B, n*n);
            fill (C, n*n);

            time = 0;
            for(int i = 0; i < iterations; i++){
                start_time = wall_time();
                square_dgemm_blocked_naive(n, A, B, C, block_sizes[iBlock]);
                time += wall_time() - start_time;
            }
            time = time / iterations;
            FILE *fp;
            printf("%i %f %i\n", n, time, block_sizes[iBlock]);
            fp = fopen("../output-files/blocked-pthreads.txt", "a");
            fprintf(fp, "%i %f %i\n", n, time, block_sizes[iBlock]);
            fclose(fp);
        }
    }
    printf("Done with Blocked Pthreads version");


    free (buf);
    return 0;
}
