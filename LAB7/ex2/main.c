/*
Descrição:
    Programa multithread com uma solucao para o problema do PapaiNoel.
    Na linha 67 tem um while() comentado para que o programa funcione totalmente como especificado.
    Os elfos demoram alguns segundos para comecar a pedir ajuda, isto eh devido ao sleep()

Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    https://github.com/caiotheodoro/SO/commits/master/LAB7/ex1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "resource_monitor.h"

/* --- funcoes --- */
void* noel();
void* rena(void* thread_id);
void* elfo(void* thread_id);

/* --- main --- */
int main(int argc, char* argv[]){
    if(argc < 2) return printf("./$ <num_de_elfos>\n");
    int numElfos = atoi(argv[1]);

    // Identificadores para as Threads. (1 noel + 9 renas + numElfos)
    int id[(1+9+numElfos)];
    for (int i=0; i < (1+9+numElfos); i++) id[i] = i;

    pthread_t papainoel;
    pthread_t renas[9];
    pthread_t elfos[numElfos];

    initMonitor();

    //criar thread noel
    pthread_create(&papainoel, NULL, noel, NULL);

    //criar threads renas
    for(int i=0; i<9; i++)
        pthread_create(&renas[i], NULL, rena, (void*) &id[i+1]);
    
    //criar threads elfos
    for(int i=0; i<numElfos; i++)
        pthread_create(&elfos[i], NULL, elfo, (void*) &id[i+10]);

    // Wait for the threads to finish
    pthread_join(papainoel, NULL);
    for(int i=0; i<9; i++) pthread_cancel(renas[i]);
    for(int i=0; i<numElfos; i++) pthread_cancel(elfos[i]);

    destroyMonitor();

    printf("Programa finalizado.\n");
    pthread_exit(0);
}


/* --- funcoes ---*/
void* noel(){
    TIPO tipo = -1;

    //while(1){             // loop infinito. (o problema dos elfos eh resolvido apos as renas)
    while(tipo != RENA){    //parar o loop apos entregar os presentes
        
        //papai noel espera por um sinal, verifica quem o chamou e faz as operacoes
        //Retorna o tipo de quem o chamou, se foi rena ou foi elfo 
        tipo = take();

        if(tipo == RENA)
            printf("NOEL: Todas as renas chegaram, Papai Noel foi entregar os presentes.\n");

        else if(tipo == ELFO)
            printf("NOEL: Resolveu o problema de 3 elfos.\n");

        fflush(stdout);
    }
}


void* rena(void* thread_id){ // 9
    int id = *((int*)thread_id);

    //seed do rand()
    srand(id);
    int tempo;

    while(1){
        // dormir por um tempo aleatorio
        tempo = (rand() % 100) / 2;
        sleep(tempo);

        // rena chegou de f'erias
        int status = put(RENA, id);
        if(status == 0) printf(">RENA %d: Chegou das ferias.\n", id);
        fflush(stdout);
    }
}


void* elfo(void* thread_id){
    int id = *((int*)thread_id);

    //seed do rand()
    srand(id);
    int tempo;

    while(1){
        // dormir por um tempo aleatorio
        tempo = (id/2) + (rand() % id);
        sleep(tempo);

        // elfo pediu ajuda
        int status = put(ELFO, id);
        if(status == 0) printf(" ELFO %d: Precisa de ajuda.\n", id);
        fflush(stdout);
    }
}