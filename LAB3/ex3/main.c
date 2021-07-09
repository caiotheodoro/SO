// Programa com N threads que calcula a mediana das linhas de uma matriz
// e a média das suas colunas. além de calcular seu tempo de execução.
// Ademais, o programa salva os resultados obtidos em um arquivo.

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
// Datas: github.com/caiotheodoro/SO/commits/master

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

int threads;

int main(int argc, char *argv[]){
    if(argc < 2) return printf("./$ <N_Threads>\n");
    threads = atoi(argv[1]);

    /* tempo de exec header */
    clock_t begin = clock();

   
    char res;


    int r, c;
    // geraMatriz(A,B,C) cria uma matriz AxB com numeros aleatorios de modulo C
    geraMatriz(1000,1000,2000);

    // le a matriz gerada na linha anteior
    int** matrix = read_matrix_from_file("matriz_entrada.in", &r, &c);
    // print_matrix(matrix, r, c);

    // vetores com os valores da media e mediana da matriz
    float media[c];
    float mediana[r];

    //cria vetores de datachunk para enviar nas pthreads
    //qtde 1 se for monothread ou varios se for multithread
    int t_qtde = 1;
    if(threads > 1) t_qtde = threads/2+1;
    dc_t* chunkMedia[t_qtde];
    dc_t* chunkMediana[t_qtde];

    pthread_t t[threads];
    //print_matrix(chunkMedia->matrix, chunkMedia->r, chunkMedia->c);


    /*  como a ideia e utilizar o main como uma thread, a chamada da funcao da main
        altera o desempenho do programa, portanto para melhorar o desempenho, tive que
        alterar os loops para que a chamada da funcao no main fosse a ultima */
    
    //delega as funcoes para as threads.
    if(threads > 1){
        // caso seja multithread
        int ini, fim; //variaveis de controle para saber qual parte dos dados deve ser lida pela thread
        int t1 = threads/2; //numero de threads para mediana
        int t2 = threads-(threads/2); //numero de threads para media

        //designa as threads para calcular a mediana
        for(int i=0; i<t2; i++){
            //calcula os valores de controle dos dados
            ini = i * (r/t2);
            fim = (i+1 == t2) ? fim = r : (i+1) * (r/t2);

            //cria uma pthread para calcular a mediana
            chunkMediana[i] = dc_criaData(matrix, r, c, ini, fim, mediana);
            pthread_create(&t[i+t2], NULL, calcMediana, (void*)chunkMediana[i]);
            printf("mediana %d\n", i);
        }

        //designa as threads para calcular a media
        for(int i=t1-1; i>=0; i--){
            //calcula os valores de controle dos dados
            ini = i * (c/t1);
            fim = (i+1 == t1) ? fim = c : (i+1) * (c/t1);

            if(i == 0){
                //a thread 0 sempre rodara na main
                //criaData(matriz, linhas, colunas, PrimeiraLinha/ColunaASerLida, UltimaLinha/ColunaASerLida, VetorDeResposta)
                chunkMedia[0] = dc_criaData(matrix, r, c, ini, fim, media); //cria o datachunk
                calcMedia((void*)chunkMedia[0]);
                printf("media %d\n", i);
            }else{
                //para i > 0, calcular a mediana em pthread
                chunkMedia[i] = dc_criaData(matrix, r, c, ini, fim, media);
                pthread_create(&t[i], NULL, calcMedia, (void*)chunkMedia[i]);
                printf("media %d\n", i);
            }
        }


    //caso threads = 1 (monothread)
    }else{
        //roda o CalcMedia no main, portanto 1 thread so
        chunkMedia[0] = dc_criaData(matrix, r, c, 0, c, media);
        calcMedia((void*)chunkMedia[0]);

        //roda o CalcMediana no main
        chunkMediana[0] = dc_criaData(matrix, r, c, 0, r, mediana);
        calcMediana((void*)chunkMediana[0]);
    }

    //sincronizacao das threads
    for(int i=0; i<threads; i++){
        if(i!=0) pthread_join(t[i], NULL);
        printf("Thread %ld sincronizada.\n", t[i]);
    }

    //printf vetores de media e mediana
    printf("\nhead (10%%) - media das colunas (em ordem): \n");
    for(int i=0; i<(c*0.1); i++) printf("%2.f, ", media[i]);

    printf("\n\nhead (10%%) - mediana das linhas (em ordem): \n");
    for(int i=0; i<(r*0.1); i++) printf("%.2f, ", mediana[i]);
    printf("\n");
    

    printf("main finalizado\n");

    /* fim do calculo do tempo de exec*/
    clock_t end = clock();
    double time_spent = (double)((end - begin)*1000)/CLOCKS_PER_SEC;
    printf("Tempo de exec em milisegundos: %f\n", (float)time_spent);


    //-------------------------------------------------------------------------------
    /*salvar arquivo*/
    printf("deseja salvar em um arquivo? (s/n)");
    scanf("%c",&res );
    res = tolower(res);
    if(res == 's'){
        //salvar no arquivo
        FILE* pFile; 
        char* insert;
        pFile = fopen("matriz_resposta.in","w"); //abre o arquivo no modo escrita
        if(pFile){
            fputs("Média\n\n",pFile); //insere "Média" no arquivo
            for(int i=0; i<c; i++) {
                sprintf(insert,"%f",media[i]); //converte o float e insere em insert
                fputs(insert,pFile); //insere no arquivo
                fputs("\n",pFile); //pula linha no arquivo
            }
                
            fputs("\n\nMediana\n\n",pFile); //insere "Mediana" no arquivo
            for(int i=0; i<r; i++) {
                sprintf(insert,"%f",mediana[i]); //converte o float e insere em insert
                fputs(insert,pFile); //insere no arquivo
                fputs("\n",pFile); //pula linha no arquivo
            }

            sprintf(insert, "\nTempo de exec em segundos: %f\n", (float)time_spent);//converte o float e insere ele mais o texto em insert
            fputs(insert,pFile); //insere no arquivo
        }
        fclose(pFile); //fecha o arquivo
    }
    printf("Fim de execução.\n");
    //-------------------------------------------------------------------------------
    
    pthread_exit(0);
}


void* calcMedia(void* data){
    dc_t* chunk = (dc_t*)data;

    for(int i=chunk->ini; i<chunk->fim; i++){ //roda as colunas
        int soma = 0;
        for(int j=0; j<chunk->r; j++){ //roda as linhas
            soma += chunk->matrix[j][i]; //soma cada valor da linha
        }

        //salva no vetor a media da coluna
        chunk->array[i] = (float)soma/chunk->r; 
    }

}

void* calcMediana(void * data){
    dc_t* chunk = (dc_t*)data;
    
    //roda as linhas
    for(int i=chunk->ini; i<chunk->fim; i++){ 

        //ordenar as linhas (bubble sort)
        for(int k=1; k<chunk->c; k++){
            for(int j=0; j<chunk->c - k; j++){
                //swap
                if(chunk->matrix[i][j] > chunk->matrix[i][j+1]){
                    int temp = chunk->matrix[i][j];
                    chunk->matrix[i][j] = chunk->matrix[i][j+1];
                    chunk->matrix[i][j+1] = temp;
                }
            }
        }

        float mediana = -1;

        //se for par, mediana = media dos dois valores do meio.
        if(chunk->c % 2 == 0){ 
            mediana = (float)(chunk->matrix[i][chunk->c/2 - 1] + chunk->matrix[i][chunk->c/2]) / 2;
        
        //se for impar, mediana = o valor do meio.
        }else{ 
            mediana = (float)chunk->matrix[i][chunk->c/2];
        }

        //salva no vetor o valor da mediana desta coluna
        chunk->array[i] = mediana;
    }
}

