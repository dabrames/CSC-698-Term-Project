
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


 double* matA;
 double* matB;
 double* matC;

int nthreads = 1;
int maxsize = 0;

void* worker (void* arg){
	
	int tid = *(int *)(arg); // get the thread ID assigned sequentially.
	int start = tid * (maxsize / nthreads);
	int end = (tid + 1) * (maxsize / nthreads);

	//printf("Worker thread %d",tid);

	for (int i = start; i < end; i++) 
        for (int j = 0; j < maxsize; j++){ 
        	double cij = matC[i+j*maxsize];
            for (int k = 0; k < maxsize; k++) 
                cij += matA[i + k*maxsize] * matB[k + j*maxsize];
            matC[i+j*maxsize] = cij;
        }
}

void square_dgemm_naive_pthreads(int size, double* a, double* b, double* c, int threads){

	matA = a;
	matB = b;
	matC = c;
	nthreads = threads;
	maxsize = size;

	//printf("Inside naive_pthreads method\n");
	//Declare number of pthreads
	pthread_t worker_threads[nthreads];

	//printf("Creating pthtreads\n");
	//Creating pthreads and starting them
	for(int i = 0; i < nthreads; i++){
		int *tid;
		tid = (int *) malloc(sizeof(int));
		*tid = i;
		//printf("Phread with id = %d", *tid);
		pthread_create(&worker_threads[i], NULL, worker, (void *) tid);
	}

	//printf("Joining pthreads\n");
	for(int i=0; i < nthreads; i++){
		pthread_join(worker_threads[i], NULL);
	}

}