#include <stdlib.h> // For: exit, drand48, malloc, free, NULL, EXIT_FAILURE
#include <stdio.h>  // For: perror
#include <string.h> // For: memset

#include <float.h>  // For: DBL_EPSILON
#include <math.h>   // For: fabs

#ifdef GETTIMEOFDAY
#include <sys/time.h> // For struct timeval, gettimeofday
#else
#include <time.h> // For struct timespec, clock_gettime, CLOCK_MONOTONIC
#endif

/* Your function must have the following signature: */
extern const char* dgemm_desc;
extern void square_dgemm (int, double*, double*, double*);
extern void square_dgemm_naive (int, double*, double*, double*);
#define MAX_SPEED 42.9  // defining Bridges Max Gflops/s per core with peak TurboBoost Frequency

// outputs a given matrix (1-D array) to"results.csv"
// results = the 1-D matrix
// n = the size of one side of the matrix
// NOTE: only for nxn matrices
void matrix_csv(double* results, int n, char* filename) {
    FILE* fp;
    fp = fopen(filename, "w+");

    printf("Printing C matrix to csv file\n");

    // for each row
    for (int i=0; i < n; ++i) {

        // print remaining columns
        for (int j=0; j < n; ++j) {
            printf("%f, ", results[i + j * n]);
            fprintf(fp, "%f, ", results[i + j * n]);
        }

        // print newline between rows
        printf("\n");
        fprintf(fp, "\n");
    }
        fclose(fp);

        printf("Results file written\n");
}

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

void absolute_value (double *p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i] = fabs (p[i]);
}

int main() {

    //Benchmarking sample sizes
    //Make sure to have the sizes in increasing order
    int test_sizes[] = { 31, 32, 96, 97, 127, 128, 129, 191, 192, 229, 255, 256, 257,
        319, 320, 321, 417, 479, 480, 511, 512, 639, 640, 767, 768, 769 };

    //File pointers for writing out the results to file
    FILE* fp;
    char* outputFile = "output.txt";
    char* benchmark_C = "benchmark_C.csv";
    char* naiveFile = "naiveC.txt";
    char* simd = "simdC.txt";
    
    int nSizes = sizeof(test_sizes)/sizeof(test_sizes[0]);

    //Hoping that last size is also the largest size
    int nMax = test_sizes[nSizes-1];

    //Allocating buffer for input and output matrices
    double* buf = NULL;
    buf = (double*) malloc (3 * nMax * nMax * sizeof(double));
    if (buf == NULL) die ("failed to allocate largest problem size");
    
    // Using iterations for averaging out the result
    int iterations = 10;

    //For each test size
    double start_time, time, delta;
    for (int iSize = 0; iSize < nSizes; ++iSize)
    {
        int n = test_sizes[iSize];
        double* A = buf + 0;
        double* B = A + nMax*nMax;
        double* C = B + nMax*nMax;
        
        fill (A, n*n);
        fill (B, n*n);
        fill (C, n*n);

        //First running the brute force
        time = 0;
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm_naive(n, A, B, C);
            time += wall_time() - start_time;
        }
        time = time / iterations;
        printf ("Naive: Size:%i, Time:%f\n", n,time);

        fp = fopen(outputFile, "w+");
        fprintf(fp, "Naive: Size:%i, Time:%f\n", n,time);
        fclose(fp);

        //Print matrix C to results.csv for comparison
        matrix_csv(C, n, benchmark_C);

       //Running simd
        time = 0;
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm (n, A, B, C);
            time += wall_time() - start_time;
        }
        time = time / iterations;
        
//        printf ("Blocked: %f, Naive: %f, Delta: %f  Size: %i  \n", time, naive_time , delta, n);
    }
    
    free (buf);
    return 0;
}
