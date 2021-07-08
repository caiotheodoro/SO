#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "random_vector.h"
#include "thread.h"

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
  for(int i = thread->pos_i;i < thread->pos_f;i++){
    if(thread->v1[i] == chave){
      return printf("Thread id: %ld, Posição inicio %d, Posição final: %d\n", pthread_self(),thread->pos_i, thread->pos_f);
    }
  }
}

int main(int argc, char *argv[])
{
    int N=argv[1];
    int tamVetor=argv[2];
    int valorBuscar = argv[3];

    rv* v1 = rv_gerar(tamVetor, 10000);


    pthread_t t1[N];
   

  for(int i=0;i<N-1;i++){
    Data *thread = criaData(i*(tamVetor/N),(i+1)*(tamVetor/N),v1, valorBuscar);

    pthread_create(&t1[i], NULL, new_thread,  &v1->array[i]);
    pthread_join (t1[i], NULL);
  
  }


   


    return 0;
}