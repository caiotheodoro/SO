// Programa com 2 threads que calcula a frequência de elementos em um vetor com valores de 0 a 9

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
// Datas: github.com/caiotheodoro/SO/commits/master

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "random_vector.h"

int frequencia[2][10];

typedef struct Data {
    int pos_i, pos_f;
    int *array;
    int seq;
}Data;


Data* criaData(int ini, int fim, int* array, int seq){
  Data* novo = (Data*)malloc(sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->array = array;
  novo->seq = seq;
  return novo;
}


void* new_thread(void* param){
  struct Data* thread = param;

  for(int i = thread->pos_i; i < thread->pos_f; i++){
    if(thread->array[i] >= 0 && thread->array[i] <= 9){
      frequencia[thread->seq][thread->array[i]]++;
    }
  }

}


int main()
{
  int tamVetor = 100000;
  int N = 2;
  rv* vtr = rv_gerar(tamVetor, 10);
  pthread_t t1[2];

  // zera todas frequencias
  for(int i = 0; i<2; i++){
    for(int j = 0; j<10; j++){
      frequencia[i][j] = 0;
    }
  }

  // obtem os dados
  for(int i=0;i<N;i++){

    int ini = i*(tamVetor/N);
    int fim = (i+1)*(tamVetor/N)-1;
    if((i+1) >= N) fim = tamVetor;

    Data* data_chunk = criaData(ini, fim, vtr->array, i);
    // printf("v %d, ini %d, fim %d, 1 %d, 2 %d, 3 %d\n", data_chunk->seq, data_chunk->pos_i, data_chunk->pos_f,data_chunk->array[1], data_chunk->array[2], data_chunk->array[3]);
    
    pthread_create(&t1[i], NULL, new_thread,  (void*) data_chunk);
    // pthread_join (t1[i], NULL);

  }

  pthread_join (t1[0], NULL);
  pthread_join (t1[1], NULL);

  // calcula e printa as frequências
  for(int i=0; i<10; i++){
    int freq = frequencia[0][i]+frequencia[1][i];
    printf("Total de '%d' no vetor: %d \t-> Frequencia: %.2f%% \t-> (t1 %d + t2 %d)\n", i, freq, (float)(freq*100)/tamVetor, frequencia[0][i], frequencia[1][i]);
  }

  printf("main finalizado\n");
  pthread_exit(0);
  return 0;
}