//
// Starting code for the portfolio exercise.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "portfolioExercise_extra.h"


// structure to store what each thread needs
typedef struct {

    int startRow;
    int endRow;

    int N;

    float **M;
    float *u;
    float *v;

    float partialDot;

} threadArgs;


// function each thread runs
void *threadWorker(void *arg)
{
    threadArgs *data = (threadArgs *)arg;

    data->partialDot = 0.0f;

    // compute assigned rows of M*u
    for(int row = data->startRow; row < data->endRow; row++)
    {
        data->v[row] = 0.0f;

        for(int col = 0; col < data->N; col++)
        {
            data->v[row] += data->M[row][col] * data->u[col];
        }

        // compute part of dot product
        data->partialDot += data->v[row] * data->v[row];
    }

    return NULL;
}


int main( int argc, char **argv )
{
    int N, nThreads;

    if( parseCmdLineArgs(argc,argv,&N,&nThreads)==-1 )
        return EXIT_FAILURE;

    float **M, *u, *v;

    if( initialiseMatrixAndVector(N,&M,&u,&v)==-1 )
        return EXIT_FAILURE;

    if( N<=12 )
        displayProblem( N, M, u, v );

    struct timespec startTime, endTime;

    clock_gettime( CLOCK_REALTIME, &startTime );


    float dotProduct = 0.0f;


    // create thread arrays
    pthread_t *threads = malloc(nThreads * sizeof(pthread_t));

    threadArgs *args = malloc(nThreads * sizeof(threadArgs));

    int rowsPerThread = N / nThreads;


    // create threads
    for(int t = 0; t < nThreads; t++)
    {
        args[t].startRow = t * rowsPerThread;

        args[t].endRow = (t+1) * rowsPerThread;

        args[t].N = N;

        args[t].M = M;

        args[t].u = u;

        args[t].v = v;

        args[t].partialDot = 0.0f;

        pthread_create(&threads[t], NULL, threadWorker, &args[t]);
    }


    // join threads and combine results
    for(int t = 0; t < nThreads; t++)
    {
        pthread_join(threads[t], NULL);

        dotProduct += args[t].partialDot;
    }


    free(threads);

    free(args);


    printf( "Result of parallel calculation: %f\n", dotProduct );


    clock_gettime( CLOCK_REALTIME, &endTime );

    double seconds =
    (double)(
        endTime.tv_sec +
        1e-9*endTime.tv_nsec -
        startTime.tv_sec -
        1e-9*startTime.tv_nsec
    );

    printf( "Time for parallel calculations: %g secs.\n", seconds );


    // serial check
    for( int row=0; row<N; row++ )
    {
        v[row] = 0.0f;

        for( int col=0; col<N; col++ )
            v[row] += M[row][col] * u[col];
    }


    float dotProduct_serial = 0.0f;

    for( int i=0; i<N; i++ )
        dotProduct_serial += v[i]*v[i];


    printf( "Result of the serial calculation: %f\n", dotProduct_serial );


    freeMatrixAndVector( N, M, u, v );

    return EXIT_SUCCESS;
}