#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include "random_vector.h"

int main(int argc, char* argv[]){
    if(argc < 2) return printf("./main <numero-de-filhos> <numero-a-procurar>\n");
    
    int tamvetor = pow(10,5);
    rv* vtr = rv_gerar(tamvetor, 10000);

    int wanted = atoi(argv[2]); //num a procurar
    int children = atoi(argv[1]); //num de filhos
    int childNum = 0; //num do processo

    printf("Iniciando: children %d - wanted %d\n", children, wanted);
    int pai = getpid();

    for(int i = 0; i < children; i++){
        int pid = fork();
        if(!pid){
            childNum++;
            if(childNum > children || childNum > 10) break;
        }else{
            break; //pare se for pai, continue se for filho    
        }
    }

    int ini = (tamvetor/children) * childNum;
    int fim = ((tamvetor/children) * (childNum + 1)) - 1;
    fim = (fim > vtr->size) ? vtr->size : fim;
    //printf("childnum %d - ini %d - fim %d\n", childNum, ini, fim);

    for(int i = ini; i < fim; i++){
        if(vtr->array[i] == wanted){
            printf("Processo %d (PID: %d) encontrou o valor %d no index %d do vetor.\n", childNum, getpid(), wanted, i);
        }
    }

    rv_excluir(vtr);

    int status;
    wait(&status);

    printf("Processo %d finalizado.\n", childNum);
}