#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

#define MAX 5			// limite do buffer

int buffer[MAX];		// buffer circular
int prox_ins = 0;		// posição de insercao
int prox_rem = 0;		// posição de remocao
int dado = 0;

sem_t mutex,            // controla acesso a SC
      pos_vazia,        // controla o acesso produtores ao buffer
      pos_cheia;        // controla o acesso consumidor ao buffer

 
void* produtor(void* data)
{
    while(1)
    {
        sem_wait(&pos_vazia);   //se existe pos vazia entra, senão bloqueia
        printf("Produzir\n");
        sem_wait(&mutex);       // inicio SC - exclusao mutua
        printf("Produzido %d\n", dado);
        sleep(1);
        buffer[prox_ins] = dado++;
        prox_ins = (prox_ins+1) % MAX;
        sem_post(&mutex);       // fim SC   
        sem_post(&pos_cheia);   // colocou valor no buffer, libera 1 consumidor 
    }
}

void* consumidor(void* data)
{
    while(1)
    {
        sem_wait(&pos_cheia);   //se existe pos cheia entra, senão bloqueia
        printf("Consumir\n");
        sem_wait(&mutex);       // inicio SC - exclusao mutua
        printf("Consumido %d\n", buffer[prox_rem]);
        sleep(1);
        prox_rem = (prox_rem+1) % MAX;
        sem_post(&mutex);       //fim SC
        sem_post(&pos_vazia);
    }
}
 
int main()
{
    pthread_t t1,t2,t3;
    
    sem_init(&mutex, 0, 1);
    sem_init(&pos_vazia, 0, MAX);
    sem_init(&pos_cheia, 0, 0);
 
    pthread_create(&t1,NULL,produtor,NULL);
    pthread_create(&t2,NULL,consumidor,NULL);
    pthread_create(&t3,NULL,produtor,NULL);
    pthread_join(t1,NULL);
    
    return 0;
}


