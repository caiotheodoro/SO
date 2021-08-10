/**
 * Realiza o incremento de uma variavel compartilhada - counter
 *  Fonte: http://www.amparo.net/ce155/sem-ex.html 
 */

#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <pthread.h>    /* POSIX Threads */
#include <semaphore.h>  /* Semaphore */

#define NUMTHREAD 2

/* prototype for thread routine */
void handler ( void *ptr );

/* semaforo declarado como global */
sem_t mutex;  // atua como um mutex
int counter;  // variavel compartilhada

int main()
{
    int id[NUMTHREAD];
    pthread_t thread[NUMTHREAD];
    
    // Identificadores para as Threads.
    for (int i=0; i < NUMTHREAD; i++) id[i] = i; 
    
    sem_init(&mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
                                 /* second param = 0 - semaphore is local */
                                 
    /* criar as threads */
    for (int i=0; i < NUMTHREAD; i++) {
        pthread_create (&thread[i], NULL, (void *) &handler, (void *) &id[i]);
    }

    for (int i=0; i < NUMTHREAD; i++) {
        pthread_join(thread[i], NULL);
    }
                                

    sem_destroy(&mutex); /* destroy semaphore */
                  
    return 0;
} /* main() */

void handler ( void *ptr )
{
    int x; 
    x = *((int *) ptr);
    printf("Thread %d: Waiting to enter critical region...\n", x);
    sem_wait(&mutex);       /* down semaphore */
    /* START CRITICAL REGION */
    printf("Thread %d: Now in critical region...\n", x);
    printf("Thread %d: Counter Value: %d\n", x, counter);
    printf("Thread %d: Incrementing Counter...\n", x);
    counter++;
    printf("Thread %d: New Counter Value: %d\n", x, counter);
    printf("Thread %d: Exiting critical region...\n", x);
    /* END CRITICAL REGION */    
    sem_post(&mutex);       /* up semaphore */
    
    pthread_exit(0); /* exit thread */
}
