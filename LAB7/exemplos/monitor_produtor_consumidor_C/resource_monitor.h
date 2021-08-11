#include <pthread.h>

/* --- concurrency controls --- */
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;

/* --- resource --- */
int buffer;
int empty;   // 1 true, 0 false

/* --- monitor operations --- */
void initMonitor();
void destroyMonitor();
void put(int value);
int take();
