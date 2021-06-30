
#include <unistd.h>      /* fork(), execv(), getpid(), getppid() */
// #include <sys/types.h>   /* pid_t type */
// #include <errno.h>       /* perror() */
#include <sys/wait.h>    /* wait() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){ //argv[0] == ./main
    if(argc < 2) return printf("erro nenhum comando de sistema inserido\n"); 

    int child = fork();

    if(!child){ //filho
        char cmd[250];
        for(int i = 1; i < argc; i++){
            strcat(cmd, argv[i]);
            strcat(cmd, " ");
        }

        system(cmd);
        printf("------------------\nchild id %d, done.\n", getpid());
    }else{ //pai

        int status;
        wait(&status);
        printf("father id %d, done. \nchild status: %d\n", getpid(), status);
    }
}

// int global = 0;       /* variável global */

// int main()
// {
//     pid_t child_pid;  /* pid do processo filho */
//     int status;
//     int local = 0;    /* variável local */ 
    
//     /* cria um novo processo */
//     child_pid = fork();

//     if (child_pid >= 0) /* sucesso do fork */
//     {
//         if (child_pid == 0) /* fork() devolve 0 para o processo filho */
//         {
//             printf("FILHO: iniciando execução!\n");

//             // incrementa as variáveis (local e a global)
//             local++;
//             global++;

//             printf("FILHO: child PID =  %d, parent pid = %d\n", getpid(), getppid());
//             printf("\nFILHO: child's local = %d, child's global = %d\n",local,global);

//             /* executa um processo */
//             char *path_cmd = "/usr/bin/whoami";
//             char *cmd[] = {"whoami",(char*)0};
//             return execv(path_cmd,cmd); // chama o programa "whoami"
//          }
//          else /* processo pai */
//          {
//              printf("PAI: continuando o processamento!\n");
//              printf("PAI: parent PID =  %d, child pid = %d\n", getpid(), child_pid);
//              wait(&status); /* espeara o filho finalizar e armazena o status do exit */
//              printf("PAI: Child exit code: %d\n", WEXITSTATUS(status));

//              //os valores das variáveis locais e globais pelo filho não alteram do pai
//              printf("\nPAI:Parent'z local = %d, parent's  global = %d\n",local,global);

//              printf("PAI: Bye!\n");
//              exit(0);  /* pai exits */
//          }
//     }
//     else /* no caso de falha do fork */
//     {
//         perror("fork");
//         exit(0);
//     }
// } 
