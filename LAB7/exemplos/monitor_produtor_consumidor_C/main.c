#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "resource_monitor.h"

#define MAX 20

/* Produtor -- Thread */
void *producer(void *ptr)
{
    for (int i = 1; i <= MAX; i++)
    {
        printf("produzindo buffer: %d\n", i);
        put(i);
        sleep(1); /* tempo para produzir um item */
    }
    pthread_exit(0);
}

/* Consumidor -- Thread */
void *consumer(void *ptr)
{
    for (int i = 1; i <= MAX; i++)
    {
        printf("consumindo buffer: %d\n", take());
    }
    pthread_exit(0);
}

/** MAIN **/
int main(int argc, char **argv)
{
    pthread_t pro, con;

    // Initialize monitor
    initMonitor();

    // Create the threads
    pthread_create(&con, NULL, consumer, NULL);
    pthread_create(&pro, NULL, producer, NULL);

    // Wait for the threads to finish
    pthread_join(con, NULL);
    pthread_join(pro, NULL);

    // Destroy monitor
    destroyMonitor();
}
