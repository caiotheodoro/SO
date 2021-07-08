// utiliza N threads na busca e identificação de determinado valor

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
// Datas: github.com/caiotheodoro/SO/commits/master

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "random_vector.h"
// #include "thread.h"

typedef struct Data {
    int pos_i, pos_f;
    int *array;
    int chave;
    int seq;
}Data;


Data* criaData(int ini, int fim, int* array, int chave, int seq){
  Data* novo = (Data*)malloc(sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->array = array;
  novo->chave = chave;
  novo->seq = seq;
  return novo;
}


void * new_thread(void* param){
  struct Data* thread = param;
  for(int i = thread->pos_i; i < thread->pos_f; i++){
    if(thread->array[i] == thread->chave){
      printf("Thread %d (ID %ld): Encontrado valor %d no index %d do vetor.\n", thread->seq, pthread_self(), thread->chave, i);
    }
  }
}


int main(int argc, char *argv[])
{
  if(argc < 4) return printf("./$ <N_Threads> <valorBusca>");

  int N = atoi(argv[1]);
  int valorBuscar = atoi(argv[2]);

  int tamVetor = N*50000;
  rv* vtr = rv_gerar(tamVetor, 25000);

  pthread_t t1[N];


  for(int i=0;i<N;i++){

    int ini = i*(tamVetor/N);
    int fim = (i+1)*(tamVetor/N)-1;
    if((i+1) >= N) fim = tamVetor;

    Data* data_chunk = criaData(ini, fim, vtr->array, valorBuscar, i);
      //printf("v %d, ini %d, fim %d, 1 %d, 2 %d, 3 %d\n", data_chunk[i]->chave, data_chunk[i]->pos_i, data_chunk[i]->pos_f,data_chunk[i]->array[1], data_chunk[i]->array[2], data_chunk[i]->array[3]);
    
    pthread_create(&t1[i], NULL, new_thread,  (void*) data_chunk);
    // pthread_join (t1[i], NULL);

  }

  printf("main finalizado\n");
  pthread_exit(0);
  return 0;
}