#include <pthread.h>

typedef enum {RENA, ELFO} TIPO;

/* --- concurrency controls --- */
pthread_mutex_t the_mutex;
pthread_cond_t condNoel, elfoWait, renaWait;

/* --- resource --- */
int bufferRena[8];
int renaCount;

int bufferElfo[3];
int elfoCount;

/* --- monitor operations --- */
void initMonitor();
void destroyMonitor();
int put(TIPO tipo, int thread_id);
TIPO take();
