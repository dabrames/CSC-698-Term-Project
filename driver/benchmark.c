#include <stdlib.h> // For: exit, drand48, malloc, free, NULL, EXIT_FAILURE
#include <stdio.h>  // For: perror
#include <string.h> // For: memset

#include <float.h>  // For: DBL_EPSILON
#include <math.h>   // For: fabs
#include <pthread.h>


#ifdef GETTIMEOFDAY
#include <sys/time.h> // For struct timeval, gettimeofday
#else
#include <time.h> // For struct timespec, clock_gettime, CLOCK_MONOTONIC
#endif

/* Your function must have the following signature: */
extern const char* dgemm_desc;
extern void square_dgemm (int, double*, double*, double*);
extern void square_dgemm_naive (int, double*, double*, double*);
extern void square_dgemm_naive_pthreads(int , double* , double* , double* , int);
#define MAX_SPEED 42.9  // defining Bridges Max Gflops/s per core with peak TurboBoost Frequency

char* outputFile = "output.txt";
char* benchmark_C = "benchmark_C.csv";
char* naiveFile = "naiveC.csv";
char* simd = "simdC.csv";

// outputs a given matrix (1-D array) to"results.csv"
// results = the 1-D matrix
// n = the size of one side of the matrix
// NOTE: only for nxn matrices
void matrix_csv(double* results, int n, char* filename) {
    FILE* fp;
    fp = fopen(filename, "w+");


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

char * get_buffer_csv(char* fileName){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( fileName, "rb" );
    if( !fp ) die(fileName);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    fclose(fp);
    return buffer;
}

unsigned compare_output_csv(char* fileName1, char* fileName2){
    char * output1 = get_buffer_csv(fileName1);
    char * output2 = get_buffer_csv(fileName2);
    unsigned ret = strcmp(output1, output2) == 0 ? 1:0;
    free(output1);
    free(output2);

    return ret;
}

void process_after_each_run(double *C, int n, double time, char *outputFileName_C,
                            char *benchmark_C, char* algorithmDesc){
    matrix_csv(C, n, outputFileName_C);
//    unsigned isCorrect = compare_output_csv(benchmark_C, outputFileName_C);
//    if(!isCorrect){
//        printf ("%s output is incorrect\n", algorithmDesc);
//    }
//    else {
//        FILE *fp;
//        printf("%s: Size:%i, Time:%f\n", algorithmDesc, n, time);
//        fp = fopen(outputFile, "w+");
//        fprintf(fp, "%s: Size:%i, Time:%f\n", algorithmDesc, n, time);
//        fclose(fp);
//    }

    FILE *fp;
    printf("%s: Size:%i, Time:%f\n", algorithmDesc, n, time);
    fp = fopen(outputFile, "a");
    fprintf(fp, "%s: Size:%i, Time:%f\n", algorithmDesc, n, time);
    fclose(fp);
}

int main() {

    //Benchmarking sample sizes
    //Make sure to have the sizes in increasing order
    int test_sizes[] = {10};
//            31, 32};
    //, 96, 97, 127, 128, 129, 191, 192, 229, 255, 256, 257,
     //   319, 320, 321, 417, 479, 480, 511, 512, 639, 640, 767, 768, 769 };

    //File pointers for writing out the results to file
    FILE* fp;
    
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

        printf("Starting the program\n");
        printf("Starting with Brute force\n");
        //First running the brute force
        time = 0;
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm_naive(n, A, B, C);
            time += wall_time() - start_time;
        }
        time = time / iterations;
        printf ("Naive: Size:%i, Time:%f\n", n,time);

        fp = fopen(outputFile, "a");
        fprintf(fp, "Naive: Size:%i, Time:%f\n", n,time);
        fclose(fp);

        //Print matrix C to results.csv for comparison
        matrix_csv(C, n, benchmark_C);

       //Running simd
        time = 0;
        for(int i = 0; i < iterations; i++){
            start_time = wall_time();
            square_dgemm_naive_pthreads(n, A, B, C, 4);
            time += wall_time() - start_time;
        }
        time = time / iterations;

        process_after_each_run(C, n, time, simd, benchmark_C, "SIMD");

        printf("The program has end.,,,!!\n");
        
    }
    free (buf);
    return 0;
}
