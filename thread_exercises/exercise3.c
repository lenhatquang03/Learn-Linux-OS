/*
Data parallelism: Data is divided into data elements that a processor can process in parallel. Several different processors simultaneously read and process different data elements.

(Task) Functional parallelism: A big task = smaller tasks that execute in parallel.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>

#define N 4

typedef struct
{
    int (*mat1)[N];
    int (*mat2)[N];
    int (*resultMat)[N];
    int startHalf;
    int tid;
} thread_args;

void *
multiplyHalf(void *args);
void printMatrix(int matrix[N][N]);

int main()
{
    int matrix1[N][N] = {
        {1, 2, 3, 4},
        {4, 5, 6, 7},
        {7, 8, 9, 10}};

    int matrix2[N][N] = {
        {10, 9, 8, 7},
        {7, 6, 5, 4},
        {4, 3, 2, 1}};

    // Print the input matrixes
    printf("Matrix 1:\n");
    printMatrix(matrix1);
    printf("Matrix 2:\n");
    printMatrix(matrix2);

    int resultMatrix[N][N];
    // Clean the result matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            resultMatrix[i][j] = 0;
        }
    }

    thread_args args[2];
    pthread_t threads[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    printf("The number of threads is 2.\n");

    for (int i = 0; i < 2; i++)
    {
        // Declare the arguments for the start routine of each thread
        args[i].mat1 = matrix1;
        args[i].mat2 = matrix2;
        args[i].resultMat = resultMatrix;
        args[i].startHalf = i * 2;
        args[i].tid = i + 1;
        // Create the thread
        pthread_create(&threads[i], &attr, multiplyHalf, (void *)&args[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("Result matrix:\n");
    printMatrix(resultMatrix);

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}

void *multiplyHalf(void *args)
{
    thread_args *myargs;
    myargs = (thread_args *)args;
    int (*firstMat)[N] = myargs->mat1;
    int (*secondMat)[N] = myargs->mat2;
    int (*result)[N] = myargs->resultMat;
    int half = myargs->startHalf;
    int tid = myargs->tid;

    pid_t tid1 = syscall(SYS_gettid);
    printf("Thread %d with ID %d is calculating the %d and %d columns of the result matrix.\n", tid, tid1, half + 1, half + 2);

    for (int i = 0; i < N; i++)
    {
        for (int j = half; j < half + (N / 2); j++)
        {
            for (int k = 0; k < N; k++)
            {
                result[i][j] += firstMat[i][k] * secondMat[k][j];
            }
        }
    }
    // Exit the thread
    pthread_exit(NULL);
}

void printMatrix(int matrix[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}