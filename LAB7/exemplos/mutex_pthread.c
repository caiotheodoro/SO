#include<stdio.h> 
#include<pthread.h> 
#include<unistd.h> 

#define N 3
  
pthread_t t[N];          // define N
pthread_mutex_t mutex;   // variável mutex
int counter;             // variável compartilhada

void* task(void *arg) 
{ 
    pthread_mutex_lock(&mutex);   // inicio SC
  
    counter += 1; 
    printf("Contador %d\n", counter); 
    sleep(2);                     //simula processamento
  
    pthread_mutex_unlock(&mutex); // saida SC
    pthread_exit(0); 
} 
  
int main(void) 
{
    /* inicializa mutex - inicializado como LIVRE */ 
    pthread_mutex_init(&mutex, NULL);
  
    /* inicializa threads */
    for (int i =0; i < N; i++) 
        pthread_create(&(t[i]), NULL, &task, NULL); 
  
    /* aguarda todas as threads finalizarem */
    for (int i=0; i < N; i++)
        pthread_join(t[i], NULL); 

    /* destroi mutex */
    pthread_mutex_destroy(&mutex); 
  
    return 0; 
} 

