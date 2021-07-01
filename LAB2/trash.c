//O Código tem como função criar processos do arquivo a partir do comando fork(),
// e manupulá-las, usando funções da biblioteca e funções específicas criadas.


// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
//Datas: github.com/caiotheodoro/SO/commits/master

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
// chat* args[] = path;
// args[] += argv[2]; 
// if(argv[3]) args[]+= argv[3]; //se argv[3] não for nulo, insere em args (casos de flag)
//  execv(bin_path,args); //executa os ARGS no bash
//wait(10);

//questão 3
// int L; 
// scanf("%d",&L);  //lê a divisão do vetor
// int aux= N/L; //divide o vetor por L
//  for(int i=1;i<=L;i++){  #subdivisão para L processos
//      for(j=0;j<(N/L)*i; j++){ //multiplica o vetor a cada loop até chegar a N elementos lidos.
//          if(vetor[i] == valor) //compara com o valor
//              return 1;   
//       }
//  }    

// questão 4
//char path = "/bin/ls";
//int i=2;
//char args[] = path;
//while(argv[i]!=NULL); //roda até que todos comandos do terminal tenham sido percorridas
// args[] += argv[i]; //coleta todos argumentos passado  no terminal
//  execv(bin_path,args); //executa os ARGS no bash
//wait(10);

// while permeando o código  para interface de shell simples dando opções. 

    int x;
    scanf("%d", &x);
    return 0;
}