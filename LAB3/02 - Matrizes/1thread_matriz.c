#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct data {
    int pos_m1,pos_n1;
    int pos_m2,pos_n2;
    int **matriz;
};

void *thread_calc(void *param){
    struct data *dados = param;

    int *matriz_sum=0;
    for(int i=dados->pos_m1;i<dados->pos_m2;i++){
        for(int j=dados->pos_n1;j<dados->pos_m2;j++){
            matriz_sum[i]+=dados->matriz[i][j];
        }
    }
    for(int i = 0; i < dados->pos_m2; i++)
    {
        printf("matriz_soma[%d]: %d",i,matriz_sum[i]);
    }
    
  

}




int main()
{
  printf("Soma das linhas de uma matriz");
    int m=4,n=4;
    int i=0,j=0;
    int matriz[m][n];

    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
          matriz[i][j]= rand() % 100;
        } 
    }


  struct data data1;

    data1.pos_m1=0, data1.pos_n1=0;
    data1.pos_m2=m, data1.pos_n2=n;
    data1.matriz = matriz;
  

    pthread_t t1;

    pthread_create(&t1, NULL, thread_calc, &data1);

    pthread_join (t1, NULL);



    return 0;
}