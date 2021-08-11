#include "resource_monitor.h"

void initMonitor()
{
    // Initialize the mutex and condition variables
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condc, NULL); /* Initialize consumer condition variable */
    pthread_cond_init(&condp, NULL); /* Initialize producer condition variable */

    buffer = 0;
    empty = 1;
}

void put(int value)
{
    pthread_mutex_lock(&the_mutex); /* protect buffer */
    while (!empty)             /* If there is something  in the buffer then wait */
        pthread_cond_wait(&condp, &the_mutex);

    buffer = value;
    empty = 0;

    pthread_cond_signal(&condc);      /* wake up consumer */
    pthread_mutex_unlock(&the_mutex); /* release the buffer */
}

int take()
{
    pthread_mutex_lock(&the_mutex); /* protect buffer */
    while (empty)                   /* If there is nothing in  the buffer then wait */
        pthread_cond_wait(&condc, &the_mutex);
    int value = buffer;
    empty = 1;
    pthread_cond_signal(&condp);      /* wake up producer */
    pthread_mutex_unlock(&the_mutex); /* release the buffer */

    return value;
}

void destroyMonitor()
{
    // Cleanup -- would happen automatically at end of program
    pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
    pthread_cond_destroy(&condc);      /* Free up consumer condition variable */
    pthread_cond_destroy(&condp);      /* Free up producer condition variable */
}
