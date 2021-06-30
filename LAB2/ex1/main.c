#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc < 2) return printf("ERRO: Precisa especificar o nivel da arvore.\n");
    
    int N = atoi(argv[1]);
    int arvNivel = 0;
    int arvPai[N];

    for(int i = 0; i < N; i++){
        int pai = getpid();
        int pid = fork();
        if(!pid){
            arvPai[arvNivel] = pai;
            arvNivel++;
        }      
    }
    
    for(int i = 0; i < arvNivel; i++) printf("PaiID %d -\t", arvPai[i]);
    printf("((PID: %d))\n", getpid());

    int x;
    scanf("%d", &x);
    return 0;
}