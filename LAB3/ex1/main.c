#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "random_vector.h"

typedef struct Data {
    int pos_i,pos_f;
    int *v1;
    int chave;
}Data;


Data* criaData(int ini, int fim,int*v1, int chave){
  Data* novo = (Data*) malloc (sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->v1 = v1;
  novo->chave = chave;

  return novo;
}
void * new_thread(void* param){
  int i;
  struct Data *thread = param;
  int chave = thread->chave;
  printf("ee");
  for(int i = thread->pos_i;i < thread->pos_f;i++){
    if(thread->v1[i] == chave){
      return printf("Thread id: %ld, Posição inicio %d, Posição final: %d\n", pthread_self(),thread->pos_i, thread->pos_f);
    }
  }
}

int main(int argc, char *argv[])
{
  printf("EEE ");
    int N=atoi(argv[1]); 
   printf("%d ", N);
    int tamVetor=atoi(argv[2]);
       printf("%d ", tamVetor);
    int valorBuscar = atoi(argv[3]);
     printf("%d ", valorBuscar);
    rv* v1 = rv_gerar(tamVetor, 10000);
  //   printf("%d ", v1);

    pthread_t t1[N];
   

  for(int i=0;i<N-1;i++){
    Data *thread = criaData(i*(tamVetor/N),(i+1)*(tamVetor/N),v1->array, valorBuscar);
    pthread_create(&t1[i], NULL, new_thread, &thread[i]);
    pthread_join (t1[i], NULL);
  
  }


   


    return 0;
}