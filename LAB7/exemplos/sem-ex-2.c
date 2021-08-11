/**
 * Realiza a soma de dois vetores usando NUMTHREAD threads
 * 
 * Fonte: http://www.amparo.net/ce155/sem-ex.html 
 * 
 * Possui um erro, QUAL? 
 * 
 */

#include <math.h>
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <semaphore.h>  /* Semaphore */

#define N 50
#define NUMTHREAD 3

void *run(void *ptr);

// Vetores de entrada.
float *a;
float *b;
// Vetor de saída.
float *c;

// Declaração do Semáforo.
sem_t mutex;
int posicao = 0; /* posicao do vetor a ser alterada */

int main(){
	
	int i;
    int id[NUMTHREAD];
    pthread_t thread[NUMTHREAD];
    pthread_attr_t attr;            /* atributos de thread. */
    
    // Identificadores para as Threads.
    for (i=0; i < NUMTHREAD; i++) id[i] = i; 
    
    // Tamanho em bytes de cada vetor
    size_t bytes = N*sizeof(float);
 
    // Alocar os vetores.
    a = (float*)malloc(bytes);
    b = (float*)malloc(bytes);
    c = (float*)malloc(bytes);
 
    // Inicializa os vetores.
    for(i=0; i<N; i++) {
        a[i] = sinf(i)*sinf(i);
        b[i] = cosf(i)*cosf(i);
    }

    sem_init(&mutex, 0, 1);      /* inicializa mutex com 1 - semaforo binário */
                                 /* segundo parametro = 0 - semaforo local */
  
	/* obtem os atributos padraos para threads */
    pthread_attr_init(&attr);
    
    /* criar as threads */
    for (i=0; i < NUMTHREAD; i++) {
        pthread_create (&thread[i], &attr, (void *) &run, (void *) &id[i]);
    }
    
    for (i=0; i < NUMTHREAD; i++) {
        pthread_join(thread[i], NULL);
    }
    
    // Resposta do calculo
    printf("Vetores: \n");
    for(i=0; i<N; i++){
        printf("%g + %g = %g\n", a[i], b[i], c[i]);
    }
 
    printf("FIM.\n");
 
    // Libera a memoria dos vetores.
    free(a);
    free(b);
    free(c);    

    // Destroi o semáforo.
    sem_destroy(&mutex);
                  
    return 0;
}

void *run(void *ptr){
	
  int tid = *((int*) ptr);
  printf("Thread %d working...\n", tid);
  
  while (posicao < N){
	  printf("Thread %d: Aguardando para entrar na região crítica.\n", tid);
      sem_wait(&mutex);  // obter o semaforo.
      
	  /* Seção Crítica - Inicio */
	  printf("Thread %d: Na região crítica.\n", tid);
	  printf("Thread %d: Posicao atual de alteracao do vetor: %d\n", tid, posicao);
	  
      printf("Thread %d: Atualizando o elemento c[%d]\n", tid, posicao);
	  c[posicao] = a[posicao] + b[posicao];

      printf("Thread %d: Incrementando a posicao %d.\n", tid, posicao);
	  posicao++;
	  
      printf("Thread %d: Saindo da região crítica...\n", tid);
	  /* Seção Crítica - Fim */    
	   sem_post(&mutex); // libera o semaforo
  }
  
  pthread_exit(0);
}
