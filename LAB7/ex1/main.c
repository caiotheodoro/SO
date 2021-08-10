/*
Descrição:
    Programa multithread com uma solucao para o problema do Leitor-Escritor.
    
Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    https://github.com/caiotheodoro/SO/commits/master/LAB7/ex1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h> 
#include <pthread.h>
#include <semaphore.h>

/* funcoes */
void leitor(void *ptr);
void escritor(void *ptr);

/* semaforo declarado como global */
sem_t mutex;  // atua como um mutex
sem_t mutexLeitor;

/* variaveis compartilhadas*/
int leitorlock = 1; //quando uma thread escrita chegar, ela fechara a entrada de novas threads leitoras na secao
int leitores = 0;
int buffer = -1;

int main(int argc, char *argv[]){
    if(argc < 3) return printf("./$ <N_Threads> <Proporcao>\n");
    int CHANCE = atoi(argv[2]);
    int NUMTHREAD = atoi(argv[1]);
    if(NUMTHREAD < 2) NUMTHREAD = 2;

    int id[NUMTHREAD];
    pthread_t thread[NUMTHREAD];

    // Identificadores para as Threads.
    for (int i=0; i < NUMTHREAD; i++) id[i] = i;
    
    // inicializa os semaforos
    sem_init(&mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore // second param = 0 - semaphore is local */
    sem_init(&mutexLeitor, 0, 1);      /* initialize mutex to 1 - binary semaphore // second param = 0 - semaphore is local */
    
    // criar as threads
    srand(time(NULL));
    pthread_create (&thread[0], NULL, (void*) &escritor, (void*) &id[0]); //garantindo que sempre existira um escritor
    for (int i=1; i < NUMTHREAD; i++) {
        //cria um numero aleatorio de 0 ate chance (proporcao threads escritoras pra thread totais)
        int tmp = rand() % CHANCE;

        // se tmp for 0, cria uma thread escritora, se for qualquer outro numero cria uma leitora.
        if(tmp == 0) pthread_create (&thread[i], NULL, (void*) &escritor, (void*) &id[i]);
        else pthread_create (&thread[i], NULL, (void*) &leitor, (void*) &id[i]);
    }

    //sincronizacao
    for (int i=0; i < NUMTHREAD; i++) {
        pthread_join(thread[i], NULL);
    }

    sem_destroy(&mutexLeitor); /* destroy semaphore */
    sem_destroy(&mutex); /* destroy semaphore */
    pthread_exit(NULL);
    return 1;
}

void escritor(void *ptr){
    int thread_id = *((int*)ptr);

    while(1){
        sem_wait(&mutex); /* down semaphore */

        //bloqueio para os leitores
        leitorlock = 1;

        //regiao critica
        buffer = thread_id;
        printf("Thread Escritora ID %d escreveu no buffer\n", thread_id);

        //simulacao de processamento complexo
        sleep(1);

        //desbloqueio para os leitores
        leitorlock = 0;

        //criar espera para que possamos ver as diferentes threads escritoras
        sleep(2);
        sem_post(&mutex); /* up semaphore */
        sleep(3);      
    }
}

void leitor(void *ptr){    
    int thread_id = *((int*)ptr);
    while(1){

        //se uma thread escrita nao bloqueou a leitura:
        if(leitorlock == 0){
            //regiao critica

            //manusear variavel compartilhada utilizando semaforos
            sem_wait(&mutexLeitor);
            leitores++;
            sem_post(&mutexLeitor);

            printf("ThreadLeitora ID %d leu o que ThreadEscritora ID %d escreveu. Num de leitores: %d\n", thread_id, buffer, leitores);
        
            //simulacao de processamento complexo
            sleep(1);

            //manusear variavel compartilhada utilizando semaforos
            sem_wait(&mutexLeitor);
            leitores--;
            sem_post(&mutexLeitor);
        
        //caso a leitura esteja bloqueada
        }else{
            printf("ThreadLeitora ID %d bloqueada pelo leitorlock\n", thread_id);
            sleep(1);
        }
    }
}