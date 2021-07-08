//SOURCE: http://www.mit.edu/people/proven/IAP_2000/basic_example.html
// editado por Rodrigo Campiolo 04/09/2018

 
#include <pthread.h> //pthread_t, pthread_create, pthread_detach, pthread_exit 
#include <stdio.h>   //fflush
#include <stdlib.h>  //exit
#include <unistd.h>  //sleep

char * buf = "abcdefghijklmnopqrstuvwxyz";
int num_pthreads = 4;
int count = 30;
int fd = 1;

void * new_thread(void * arg)
{
    int i;
    char *letter = (char *) arg;

    for (i = 0; i < count; i++) {
        printf("%c", *letter); 
        fflush(stdout);
        sleep(1);
    }
    return(NULL);
}

int main()
{
   pthread_t thread;
   int i;

   for (i = 0; i < num_pthreads; i++) {
        if (pthread_create(&thread, NULL, new_thread, (void *)(buf + i))) {
            fprintf(stderr, "error creating a new thread \n");
            exit(1);
        }
        pthread_detach(thread);         // desanexa a thread
        //pthread_join(thread, NULL);   // aguarda a thread terminar
   }
   
   printf("main thread finished");
   pthread_exit(NULL);               // finaliza a thread (depois de liberar os recursos)
}
