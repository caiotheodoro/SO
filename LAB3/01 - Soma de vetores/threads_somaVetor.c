#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


struct data {
    int pos_i,pos_f;
    int *v1;
    int *v2;
};


void *thread_calc(void *param){
    struct data *dados = param;

    int *v3;
    int j = dados->pos_i;
while (j <= dados->pos_f){
        v3[j]= dados->v1[j]+dados->v2[j];
        j++;
}

   /*for(int i=dados->pos_i;i<dados->pos_f;i++){
         printf("v[%d]= %d",i,v3[i]);
    }
*/
}

int main()
{
  printf("Soma de vetores");
    int i=0,size=8;
    int v1[size];
    int v2[size];

    while(i < size){
      v1[i]= rand() %100;
      v2[i]= rand() %100;
      i++;
    }


  struct data data1, data2;

    data1.pos_i = 0;
    data1.pos_f = size/2;
    data1.v1 = v1;
    data1.v2 = v2;

    data2.pos_i = size/2+1;
    data2.pos_f = size;
    data2.v1 = v1;
    data2.v2 = v2;

    pthread_t t1,t2;

    pthread_create(&t1, NULL, thread_calc, &data1);
    pthread_create(&t2, NULL, thread_calc, &data2);

    pthread_join (t1, NULL);
    pthread_join (t2, NULL);


    return 0;
}