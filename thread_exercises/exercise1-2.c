#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define NTHREADS 8
#define ARRAYSIZE 1000000
#define ITERATIONS ARRAYSIZE / NTHREADS

double sum = 0.0, a[ARRAYSIZE];
pthread_mutex_t sum_mutex;

void *do_work(void *tid)
{
    int i, start, *mytid, end;
    double mysum = 0.0;
    pid_t tid2 = syscall(SYS_gettid);

    /* Initialize my part of the global array and keep local sum */
    mytid = (int *)tid;
    start = (*mytid * ITERATIONS);
    end = start + ITERATIONS;

    for (i = start; i < end; i++)
    {
        a[i] = i * 1.0;
        mysum = mysum + a[i];
    }
    printf("Thread %d with id %d is doing iterations %d to %d with its partial sum =  %.2f\n", *mytid, tid2, start, end - 1, mysum);
    /* Lock the mutex and update the global sum, then exit */
    pthread_mutex_lock(&sum_mutex);
    sum = sum + mysum;
    /*
    Print out the contents of mytid, sum, and mysum immediately after sum = sum + mysum. */
    printf("mytid: %d\n", *mytid);
    printf("sum: %.2f\n", sum);
    printf("mysum: %.2f\n", mysum);
    printf("\n");
    pthread_mutex_unlock(&sum_mutex);
    sleep(10);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i, start, tids[NTHREADS];
    pthread_t threads[NTHREADS];
    pthread_attr_t attr;

    /* Pthreads setup: initialize mutex and explicitly create threads in a
       joinable state (for portability).  Pass each thread its loop offset */
    pthread_mutex_init(&sum_mutex, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    printf("Number of threads is %d\n", NTHREADS);
    for (i = 0; i < NTHREADS; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], &attr, do_work, (void *)&tids[i]);
    }

    /* Wait for all threads to complete then print global sum */
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("This is the sum as calculated by the threads Sum= %e \n", sum);

    sum = 0.0;
    for (i = 0; i < ARRAYSIZE; i++)
    {
        a[i] = i * 1.0;
        sum = sum + a[i];
    }
    printf("This is the verification from the main thread %e\n", sum);

    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&sum_mutex);
    pthread_exit(NULL);
}