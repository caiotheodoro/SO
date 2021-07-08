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
    int pos_i, pos_f; //demilitadores de posição para busca
    int *array; // array para buscar
    int chave; //valor a ser buscado               //scruct da thread
    int seq; // numero da thread (0,1,2..)
}Data;


Data* criaData(int ini, int fim, int* array, int chave, int seq){
  Data* novo = (Data*)malloc(sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->array = array;
  novo->chave = chave;
  novo->seq = seq;
  return novo;
}// inicializa a struct com os params passados


void * new_thread(void* param){
  struct Data* thread = param; // struct na posição i
  for(int i = thread->pos_i; i < thread->pos_f; i++){// percorre o array entre os delimitadores
    if(thread->array[i] == thread->chave){ // verifica sequencialmente se é chave
      printf("Thread %d (ID %ld): Encontrado valor %d no index %d do vetor.\n", thread->seq, pthread_self(), thread->chave, i);// printa se achou
    }
  }
}


int main(int argc, char *argv[])
{
  if(argc < 4) return printf("./$ <N_Threads> <valorBusca>");

  int N = atoi(argv[1]); // valor 1  do cmd, numero de thread
  int valorBuscar = atoi(argv[2]); // valor 2 do cmd, numero para buscar

  int tamVetor = N*50000; // tamanho do vetor (num thread * 50000)
  rv* vtr = rv_gerar(tamVetor, 25000); //gera o vetor aleatorio com modulo 25000

  pthread_t t1[N]; //inicializa os indicadores da thread


  for(int i=0;i<N;i++){

    int ini = i*(tamVetor/N); //divide a fatia inicia de busca
    int fim = (i+1)*(tamVetor/N)-1;//divide a fatia final de busca
    if((i+1) >= N) fim = tamVetor; //verifica o ultimo 

    Data* data_chunk = criaData(ini, fim, vtr->array, valorBuscar, i);// inicializa struct
      //printf("v %d, ini %d, fim %d, 1 %d, 2 %d, 3 %d\n", data_chunk[i]->chave, data_chunk[i]->pos_i, data_chunk[i]->pos_f,data_chunk[i]->array[1], data_chunk[i]->array[2], data_chunk[i]->array[3]);
    
    pthread_create(&t1[i], NULL, new_thread,  (void*) data_chunk);// cria a thread
    // pthread_join (t1[i], NULL);

  }

  printf("main finalizado\n");
  pthread_exit(0);
  return 0;
}