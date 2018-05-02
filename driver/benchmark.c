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
void print_to_csv(double* results, int n) {
    FILE* fp;
    char* filename = "results.csv";
    
    fp = fopen(filename, "w+");

    printf("Printing results file of following matrix:\n");

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
    int test_sizes[] = { 4, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
    
    int nsizes = sizeof(test_sizes)/sizeof(test_sizes[0]);

    /* assume last size is also the largest size */
    int nmax = test_sizes[nsizes-1];
    
    double* buf = NULL;
    buf = (double*) malloc (3 * nmax * nmax * sizeof(double));
    if (buf == NULL) die ("failed to allocate largest problem size");
    
    
    /* For each test size */
    double start_time, time, naive_time, delta;
    int iterations = 10;
    for (int isize = 0; isize < nsizes; ++isize)
    {
        int n = test_sizes[isize];
        double* A = buf + 0;
        double* B = A + nmax*nmax;
        double* C = B + nmax*nmax;
        
        fill (A, n*n);
        fill (B, n*n);
        fill (C, n*n);
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm_naive(n, A, B, C);
            naive_time = wall_time() - start_time;
        }
        naive_time = naive_time / iterations;
       
        
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm (n, A, B, C);
            time = wall_time() - start_time;
        }
        time = time / iterations;
        
        delta = ((time - naive_time) / naive_time) * -100;
        printf ("Blocked: %f, Naive: %f, Delta: %f  Size: %i  \n", time, naive_time , delta, n);
    }
    
    free (buf);
    return 0;
}
