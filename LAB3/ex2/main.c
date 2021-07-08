// Programa com 2 threads que calcula a frequência de elementos em um vetor com valores de 0 a 9

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
// Datas: github.com/caiotheodoro/SO/commits/master

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "random_vector.h"

int frequencia[2][10]; //vetor das frequencias

typedef struct Data {
    int pos_i, pos_f; //demilitadores de posição para busca
    int *array; // array para buscar
    int seq;// numero da thread (0,1,2..)
}Data;


Data* criaData(int ini, int fim, int* array, int seq){
  Data* novo = (Data*)malloc(sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->array = array;
  novo->seq = seq;
  return novo;
}// inicializa a struct com os params passados


void* new_thread(void* param){
  struct Data* thread = param;// struct na posição i

  for(int i = thread->pos_i; i < thread->pos_f; i++){// percorre o array entre os delimitadores
    if(thread->array[i] >= 0 && thread->array[i] <= 9){ //verificador
      frequencia[thread->seq][thread->array[i]]++; //soma a frequẽncia
    }
  }

}


int main()
{
  int tamVetor = 100000;
  int N = 2;
  rv* vtr = rv_gerar(tamVetor, 10); //gera o vetor com mod 10
  pthread_t t1[2]; //identificadores de duas threads

  // zera todas frequencias
  for(int i = 0; i<2; i++){
    for(int j = 0; j<10; j++){
      frequencia[i][j] = 0; //inicializa o vetor zerando as posições
    }
  }

  // obtem os dados
  for(int i=0;i<N;i++){

    int ini = i*(tamVetor/N);//divide a fatia inicia de busca
    int fim = (i+1)*(tamVetor/N)-1;//divide a fatia final de busca
    if((i+1) >= N) fim = tamVetor;//verifica o ultimo 

    Data* data_chunk = criaData(ini, fim, vtr->array, i);// inicializa struct
    // printf("v %d, ini %d, fim %d, 1 %d, 2 %d, 3 %d\n", data_chunk->seq, data_chunk->pos_i, data_chunk->pos_f,data_chunk->array[1], data_chunk->array[2], data_chunk->array[3]);
    
    pthread_create(&t1[i], NULL, new_thread,  (void*) data_chunk);//inicializa thread
    // pthread_join (t1[i], NULL);

  }

  pthread_join (t1[0], NULL);//sincroniza as threas
  pthread_join (t1[1], NULL);//sincroniza as threas

  // calcula e printa as frequências
  for(int i=0; i<10; i++){
    int freq = frequencia[0][i]+frequencia[1][i];//imprime frequências
    printf("Total de '%d' no vetor: %d \t-> Frequencia: %.2f%% \t-> (t1 %d + t2 %d)\n", i, freq, (float)(freq*100)/tamVetor, frequencia[0][i], frequencia[1][i]);
  }

  printf("main finalizado\n");
  pthread_exit(0);
  return 0;
}