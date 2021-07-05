#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "random_vector.h"
#include "thread.h"



int main(int argc, char *argv[])
{
    int N=argv[1];
    int tamVetor=argv[2];
    int valorBuscar = argv[3];

    rv* v1 = rv_gerar(tamVetor, 10000);


    pthread_t *t1;
   

  for(int i=0;i<N-1;i++){
    Data *thread = criaData(i*(tamVetor/N),(i+1)*(tamVetor/N),v1);

    //pthread_create(&t1[i], NULL);
    pthread_join (t1[i], NULL);
  
  }


   


    return 0;
}