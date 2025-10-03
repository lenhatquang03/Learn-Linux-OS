#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>

// Created so that "void *isPrime(void *args)"" could modify the main() method's local arrays
typedef struct
{
    bool *prime_entry;
    int tid;
} thread_args;

void *isPrime(void *args);

int main()
{
    // Take the user's input
    int N;
    printf("Check for prime numbers up to: ");
    scanf("%d", &N);

    // Dynamically allocate memory
    bool *prime = calloc(N + 1, sizeof(bool));
    pthread_t *threads = calloc(N + 1, sizeof(pthread_t));
    thread_args *args = calloc(N + 1, sizeof(thread_args));

    // Thread attributes
    pthread_attr_t attr;
    // Set default attributes
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    printf("The number of threads is %d.\n", N);

    for (int i = 0; i <= N; i++)
    {
        // Each thread would set the value of the entry prime[i] via "void *isPrime(void *args)""
        args[i].prime_entry = &prime[i];
        args[i].tid = i;
        pthread_create(&threads[i], &attr, isPrime, (void *)&args[i]);
    }

    /*
    Wait for each thread to terminate and release its resource.
    If not, then they will become zombies with no entry in the process table but remained resource in the process' stack.*/
    for (int i = 0; i <= N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i <= N; i++)
    {
        if (prime[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");

    // Free the memory and return the memory key
    free(prime);
    prime = NULL;
    free(threads);
    threads = NULL;
    free(args);
    args = NULL;
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}

/*
"void *isPrime(void *args)"" is a function that accepts ONLY 1 ARGUMENT OF TYPE VOID POINTER and returns a VOID POINTER as well.

"void *" is a generic pointer. It can point to any data type. So we can pass any pointer type to a thread and the thread itself could return any pointer type when joined.
*/
void *isPrime(void *args)
{
    thread_args *myargs;
    // Cast the pointer back to its correct type
    myargs = (thread_args *)args;
    int num = myargs->tid;
    /*
    "curr_entry" is a pointer whose value is copied from "prime_entry". And since "prime_entry" is also a pointer pointing to the boolean value stored at the address of prime[i], so is "curr_entry".
     */
    bool *curr_entry = myargs->prime_entry;
    // Go to the address that "curr_entry" points to, fetch that value, and set it to "true".
    *curr_entry = true;

    // Get the ID of the threat that's executing this function
    pid_t tid1 = syscall(SYS_gettid);
    printf("Thread %d is checking if %d is a prime number or not.\n", tid1, num);

    if ((num == 0) || (num == 1))
    {
        *curr_entry = false;
    }
    else
    {
        for (int i = 2; i * i <= num; i++)
        {
            if (num % i == 0)
            {
                *curr_entry = false;
                break;
            }
        }
    }
    pthread_exit(NULL);
}