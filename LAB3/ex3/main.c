#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "matriz.h"
#include "datachunk.h"

void* calcMedia(void* data);
void* calcMediana(void * data);
void geraMatriz(int rMax,int cMax, int vMax);
int threads;

int main(int argc, char *argv[]){

    /* tempo de exec header */
    clock_t begin = clock();
    /* fim tempo de exec header */

   
    char res;
    if(argc < 2) return printf("./$ <N_Threads>");
    threads = atoi(argv[1]);

    int r, c;
    geraMatriz(10,10,100);
    int** matrix = read_matrix_from_file("matriz.in", &r, &c);
    // print_matrix(matrix, r, c);
    // printf("r %d, c %d\n", r, c);

    float media[c];
    float mediana[r];

    int t_qtde = 1;
    if(threads > 1) t_qtde = threads/2+1;
    dc_t* chunkMedia[t_qtde];
    dc_t* chunkMediana[t_qtde];

    pthread_t t[threads];
    //print_matrix(chunkMedia->matrix, chunkMedia->r, chunkMedia->c);

    if(threads > 1){ // multithread
        int ini, fim;
        int t1 = threads/2; //numero de threads para mediana
        int t2 = threads-(threads/2); //numero de threads para media

        for(int i=0; i<t1; i++){ //for para mediana
            ini = i * (r/t1);
            fim = (i+1 == t1) ? fim = r : (i+1) * (r/t1);
            //printf("ini %d, fim %d, i %d\n", ini, fim, i);

            if(i == 0){
                // thread main
                chunkMediana[0] = dc_criaData(matrix, r, c, ini, fim, mediana);
                calcMediana((void*)chunkMediana[0]);
            }else{
                //pthread
                chunkMediana[i] = dc_criaData(matrix, r, c, ini, fim, mediana);
                //calcMediana((void*)chunkMediana[i]);
                pthread_create(&t[i], NULL, calcMediana, (void*)chunkMediana[i]);
                //pthread_join(t[i], NULL);
            }
        }

        for(int i=0; i<t2; i++){
            ini = i * (c/t2);
            fim = (i+1 == t2) ? fim = c : (i+1) * (c/t2);
            //printf("ini %d, fim %d, i %d\n", ini, fim, i);

            //pthread
            chunkMedia[i] = dc_criaData(matrix, r, c, ini, fim, media);
            //calcMedia((void*)chunkMedia[i]);
            pthread_create(&t[i+t2], NULL, calcMedia, (void*)chunkMedia[i]);
        }
           
    }else{ //rodar com 1 thread soh (main)
        chunkMedia[0] = dc_criaData(matrix, r, c, 0, c, media);
        calcMedia((void*)chunkMedia[0]);

        chunkMediana[0] = dc_criaData(matrix, r, c, 0, r, mediana);
        calcMediana((void*)chunkMediana[0]);
    }

    //printf vetor1 e vetor2
    //salvar no arquivo

    for(int i=0; i<threads; i++){
        if(i!=0) pthread_join(t[i], NULL);
        printf("Thread %ld sincronizada.\n", t[i]);
    }

    printf("\nmedia das colunas: \n");
    for(int i=0; i<c; i++) printf("%2.f\n", media[i]);

    printf("mediana das linhas: \n");
    for(int i=0; i<r; i++) printf("%.2f\n", mediana[i]);
    

    printf("main finalizado\n");

    // for(int i=0; i < 1000000000; i++);

    /* tempo de exec footer */
    clock_t end = clock();
    double time_spent = (double)((end - begin)*1000)/CLOCKS_PER_SEC;
    printf("Tempo de exec em milisegundos: %f\n", (float)time_spent);
    /* fim tempo de exec footer */

    printf("deseja salvar em um arquivo? (s/n)");
    scanf("%c",&res );
    res = tolower(res);
    if(res == 's'){
    FILE* pFile;

        pFile = fopen("matriz_resposta.in","w");
        if(pFile){
            fputs("média\n\n",pFile);
        for(int i=0; i<c; i++) {
            char* insert;
            sprintf(insert,"%f",media[i]);
            fputs(insert,pFile);
            fputs("\n",pFile);
            }
            fputs("mediana\n\n",pFile);
        for(int i=0; i<r; i++) {
            char* insert;
            sprintf(insert,"%f",mediana[i]);
            fputs(insert,pFile);
            fputs("\n",pFile);
            }
    }
    
    fclose(pFile);
    }
    else{
        printf("Fim de execução.");
    }
 

    pthread_exit(0);
}


void* calcMedia(void* data){
    dc_t* chunk = (dc_t*)data;
    //print_matrix(chunk->matrix, chunk->r, chunk->c);
    //printf("ini %d fim %d\n", chunk->ini, chunk->fim);

    for(int i=chunk->ini; i<chunk->fim; i++){ //roda as colunas
        int soma = 0;
        for(int j=0; j<chunk->r; j++){ //roda as linhas
            soma += chunk->matrix[j][i];
            //printf("matriz %d - c %d, r %d - soma %d\n", chunk->matrix[j][i], i, j, soma);
        }
        chunk->array[i] = (float)soma/chunk->r;
        //printf("soma %d, chunkr %d, media %f, chunk %f\n", soma, chunk->r, (float)soma/chunk->r, chunk->array[i]);
    }

}

void* calcMediana(void * data){
    dc_t* chunk = (dc_t*)data;
    //print_matrix(chunk->matrix, chunk->r, chunk->c);
    //printf("c %d\n", chunk->c);

    //ordenar as linhas

    for(int i=chunk->ini; i<chunk->fim; i++){ //roda as linhas
        float mediana = -1;
        if(chunk->c % 2 == 0){
            mediana = (float)(chunk->matrix[i][chunk->c/2 - 1] + chunk->matrix[i][chunk->c/2]) / 2;
        }else{
            mediana = (float)chunk->matrix[i][chunk->c/2];
        }
        chunk->array[i] = mediana;
    }
}

void geraMatriz(int rMax,int cMax, int vMax){

    FILE* pFile;
    char* insert;
    int value;
    
    rMax = rand() % rMax;
    cMax = rand() % cMax;   

    pFile = fopen("matriz.in","w");
    if(pFile){
        sprintf(insert,"%dx%d\n",rMax,cMax);
    for(int i=0; i<rMax; i++) {
        for(int j=0; i<cMax; i++) {
            value = rand() % vMax;
            sprintf(insert,"%d ",value);
            fputs(insert,pFile);
            }
            fputs("\n",pFile);
        }   
    }
fclose(pFile);
}