
//Tem como objetivo simular a interface de um bash. São inseridos um printf e fgets
// em um loop infinito até que seja digitado "exit". O processo intercepta a string digitada e
//busca nela um '&' para verificar o processo de segundo plano, caso não seja encontrado executa
//todo o código digitado, caso não, roda a substrig até o & em segundo plano, executa o
//restando usando system então reestabele o fgets.

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
//Datas: github.com/caiotheodoro/SO/commits/master


#include <unistd.h>      /* fork(), execv(), getpid(), getppid() */
#include <sys/wait.h>    /* wait() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 255

int main(int argc, char *argv[]){ 
    
    int start = 1;

    while(start != 0){
        char *comando;
        int j=0,i=0, len;
    
        printf("$ ");
        fgets(comando, N, stdin); //pega toda a string

        if(comando == "exit") return 0; //verifica se não é exit
        len = strlen(comando);
        printf("strlen(comando) %d",len);

        for(i=0;i< strlen(comando); i++){ //percorre a string
            if(comando[i] == '&'){ //procura o '&'
                char* comando1;
                memcpy(comando1,comando,i); //copia a substring até o &
                 printf("comando1 %s",comando1); 
                system(comando1); //executa

                while(i != strlen(comando)){ //joga o restante dentro de com
                    comando[j] =  comando[i+1]; //reescreve comando com o restante
                    j++;
                }    
                comando[j+1] = '\0'; //fecha a string no fim da reescrita
            }
            
        }
        printf("comando 0 %s",comando); //executa o comando caso não haja & ou o restande depois de  &
        system(comando);
         //execv(path,comando);
    }

}