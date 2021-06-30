#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc < 2) return printf("ERRO: Precisa especificar o nivel da arvore.\n");
    
    int N = atoi(argv[1]);
    int arvore = 0;
    int arvPai[N];

    for(int i = 0; i < N; i++){
        int pai = getpid();
        int pid = fork();
        if(!pid){
            arvPai[arvore] = pai;
            arvore++;
        }      
    }
    
    for(int i = 0; i < arvore; i++) printf("PaiID %d -\t", arvPai[i]);
    printf("((PID: %d))\n", getpid());

// questão 2
//char path = "/bin/ls";
//int i=2;
//char args[] = path;
//while(argv[i]!=NULL);
// args[] += argv[i];
//  execv(bin_path,args);
//wait(10);

//questão 3
// int L;
// scanf("%d",&L);
// int aux= N/L;
//  for(int i=1;i<=L;i++){  #subdivisão para L processos
//      for(j=0;j<(N/L)*i; j++){
//          if(vetor[i] == valor)
//              return 1;   
//       }
//  }    

    int x;
    scanf("%d", &x);
    return 0;
}