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


  struct data data1, data2, data3, data4, data5, data6, data7, data8;

    data1.pos_m1=0, data1.pos_n1=0;
    data1.pos_m2=m/8, data1.pos_n2=n/8;
    data1.matriz = matriz;
  
    data2.pos_m1=m/8, data1.pos_n1=n/8+1;
    data2.pos_m2=2*m/8, data1.pos_n2=2*n/8;
    data2.matriz = matriz;

    data3.pos_m1=2*m/8, data1.pos_n1=2*n/8+1;
    data3.pos_m2=3*m/8, data1.pos_n2=3*n/8;
    data3.matriz = matriz;

    data4.pos_m1=3*m/8, data1.pos_n1=3*n/8+1;
    data4.pos_m2=4*m/8, data1.pos_n2=4*n/8;
    data4.matriz = matriz;      

    data5.pos_m1=4*m/8, data1.pos_n1=4*n/8+1;
    data5.pos_m2=5*m/8, data1.pos_n2=5*n/8;
    data5.matriz = matriz;
  
    data6.pos_m1=5*m/8, data1.pos_n1=5*n/8+1;
    data6.pos_m2=6*m/8, data1.pos_n2=6*n/8;
    data6.matriz = matriz;

    data7.pos_m1=6*m/8, data1.pos_n1=6*n/8+1;
    data7.pos_m2=7*m/8, data1.pos_n2=7*n/8;
    data7.matriz = matriz;

    data8.pos_m1=7*m/8, data1.pos_n1=7*n/8+1;
    data8.pos_m2=m, data1.pos_n2=n;
    data8.matriz = matriz;
    
    pthread_t t1,t2,t3,t4,t5,t6,t7,t8;


    pthread_create(&t1, NULL, thread_calc, &data1);
    pthread_create(&t2, NULL, thread_calc, &data1);
    pthread_create(&t3, NULL, thread_calc, &data1);
    pthread_create(&t4, NULL, thread_calc, &data1);
    pthread_create(&t5, NULL, thread_calc, &data1);
    pthread_create(&t6, NULL, thread_calc, &data1);
    pthread_create(&t7, NULL, thread_calc, &data1);
    pthread_create(&t8, NULL, thread_calc, &data1);    

    pthread_join (t1, NULL);
    pthread_join (t2, NULL);
    pthread_join (t3, NULL);
    pthread_join (t4, NULL);
    pthread_join (t5, NULL);
    pthread_join (t6, NULL);
    pthread_join (t7, NULL);
    pthread_join (t8, NULL);


    return 0;
}