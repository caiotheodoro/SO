//O Código tem como função criar processos do arquivo a partir do comando fork(),
// e manupulá-las, usando funções da biblioteca e funções específicas criadas.
//A cada nivel digitado pelo usuario o numero de processos filhos duplica, fazendo
// então um hierarquia de 2^n-1 processos.


// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
//Datas: github.com/caiotheodoro/SO/commits/master

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc < 2) return printf("ERRO: Precisa especificar o nivel da arvore.\n"); //verifica se foi digitado um valor depois do /.main 
    
    int N = atoi(argv[1]); //grava o nivel digitado no bash
    int arvNivel = 0;
    int arvPai[N];

    for(int i = 0; i < N; i++){ //roda para cada nivel de arvore
        int pai = getpid(); 
        int pid = fork(); //cria um processo, duplicando a cada loop
        if(!pid){
            arvPai[arvNivel] = pai; //se não retornar PID, atribui como Pai do nivel.
            arvNivel++;
        }      
    }
    
    for(int i = 0; i < arvNivel; i++) printf("PaiID %d -\t", arvPai[i]); //imprime pai
    printf("((PID: %d))\n", getpid()); //imprime filhos

    int x;
    scanf("%d", &x);
    return 0;
}