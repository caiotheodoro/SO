/*
Descrição:
    Programa multiprocesso que gera 2 vetores aleatorios de N posicoes, soma os dois e gera um terceiro vetor
    com as respostas.
    O programa utiliza shared memory e pipes.
    Obs: Para evitar problemas de concorrencia, foi adicionado um sleep() em cada processo funcionando como um antibug.

Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    https://github.com/caiotheodoro/SO/commits/master/LAB5/ex3
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

/*recursos*/


int main(int argc, char **argv){
    //parametros
    if(argc < 3) return printf("./$ <N_Process> <Vector_Size>\n");
    int numProcess = atoi(argv[1]);
    int vtrSize = atoi(argv[2]);
    printf("Iniciando soma de dois vetores de %d elementos com %d processos.\n", vtrSize, numProcess);
    
    //shared memory cfg
    const char* memName = "LAB5-EX3";
    const int moduloCharQtde = 3;
    const int sharedMemSize = vtrSize * 3 * (moduloCharQtde+1) * sizeof(char) * 10; //tamanho do vetor * qtde de vetores (A, B e Resul) * caracteres que um numero possui;

    //criar pipes
    int buffer;
	int pipeToChild[numProcess][2];
	int pipeToFather[numProcess][2];
    for(int i=0; i<numProcess; i++){
        if(pipe(pipeToChild[i]) != 0)
            return printf("Falha ao criar o Pipe To Child.\n");
        if(pipe(pipeToFather[i]) != 0)
            return printf("Falha ao criar o Pipe To Father.\n");
    }

    //criar processos filhos
	pid_t pid;
    //int childId;
    for(int i=0; i<numProcess; i++){
        pid = fork();
        //filho
        if(!pid){ 
            pid = (pid_t) i; //utilizando o pid nulo dos filhos como identificador interno de processo;
            break;
        }      
    }

    /********************************************************************/
    /****************************** PROGRAMA ****************************/
    /********************************************************************/

    // *^*^*^*^*^*^*^*^*^*^*^*^ filho ^*^*^*^*^*^*^*^*^*^*^*^*
    // *^*^*^*^*^*^*^*^*^*^*^*^ filho ^*^*^*^*^*^*^*^*^*^*^*^*
    // *^*^*^*^*^*^*^*^*^*^*^*^ filho ^*^*^*^*^*^*^*^*^*^*^*^*
    if(pid < numProcess){
        int start, finish, signal=0;
        // Distribuicao de tarefas (ini e fim do vetor)
        close(pipeToChild[pid][PIPE_WRITE]);
        read(pipeToChild[pid][PIPE_READ], &start, sizeof(start));
        read(pipeToChild[pid][PIPE_READ], &finish, sizeof(finish));

        int dowhile = 10;
        do{
            //macacagem antibug pra consertar o problema de concorrencia
            sleep(pid);

            dowhile--;
            // Abrir shared memory
            int sharedMem = shm_open(memName, O_RDWR, 0666);
            
            //em caso de erro na shared memory (controle de concorrencia), tentar de novo
            if(sharedMem == -1) {
                //shm_unlink(memName);
                printf("Processo %d esperando (%d/250)...\n", pid, 250-dowhile);
                if(dowhile-1 == 0) return printf("Erro no shm_open: Processo %d falhou\n", pid);
                sleep(1);

            //em caso de sucesso em abrir a shared memory:
            }else{
                //inicio controle de concorrencia (not working)
                //mlockall(MCL_CURRENT);

                // Acesso a shared memory
                void* ptr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0);
                //printf("pid %d, %s\n", pid, (char*)ptr);

                // Criar os 3 vetores a partir do shared memory
                int vtr[3][vtrSize];
                char x[10]; //buffer temporario
                int k = 0;
                for(int i=0; i<3; i++){
                    for(int j=0; j<vtrSize; j++){
                        sscanf(ptr+k, "%s", x);
                        k += strlen(x) + 1;
                        vtr[i][j] = atoi(x);
                    }
                }

                // Somar vetor A + B
                int res[vtrSize];
                for(int i=start; i<=finish; i++)
                    res[i] = vtr[0][i] + vtr[1][i];

                // Inserir o resultado no shared memory
                k=0;
                for(int i=0; i<3; i++){
                    for(int j=0; j<vtrSize; j++){
                        if(i == 2 && j >= start && j <= finish){
                            k += sprintf(ptr+k, "%d ", res[j]);
                        }else{
                            k += sprintf(ptr+k, "%d ", vtr[i][j]);
                        }
                    }
                }
                //printf("pid %d, %s\n", pid, (char*)ptr);

                // unlink shared memory
                // if(shm_unlink(memName) == -1) {
                //     printf("Error removing %s\n", memName);
                //     exit(-1);
                // }

                //fim controle de concorrencia
                //munlockall();

                //sinal de finalizacao
                signal = 1;
                close(pipeToFather[pid][PIPE_READ]);
                write(pipeToFather[pid][PIPE_WRITE], &signal, sizeof(signal));
                return printf("Filho %d (pid: %d) terminou a execucao\n", (int)pid, getpid());
            }
        }while(dowhile > 0);
    }


    // *^*^*^*^*^*^*^*^*^*^*^*^ pai ^*^*^*^*^*^*^*^*^*^*^*^*
    // *^*^*^*^*^*^*^*^*^*^*^*^ pai ^*^*^*^*^*^*^*^*^*^*^*^*
    // *^*^*^*^*^*^*^*^*^*^*^*^ pai ^*^*^*^*^*^*^*^*^*^*^*^*
    // Criar memoria compartilhada
    int sharedMem = shm_open(memName, O_CREAT | O_RDWR, 0666);
    ftruncate(sharedMem, sharedMemSize);

    // Mapeia a memoria compartilhada para ptr
	void* ptr = mmap(0, sharedMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem, 0);
	if(ptr == MAP_FAILED) return printf("Map failed\n");
    
    // Controle de concorrencia
    //mlockall(MCL_CURRENT);

    // Insere os Vetores na shared memory
    int k = 0;
    int modulo = (int)pow(10, moduloCharQtde);
    for(int i=0; i<2; i++)
        for(int j=0; j<vtrSize; j++)
            k += sprintf(ptr+k, "%d ", rand() % modulo);

    for(int j=0; j<vtrSize; j++)
        k += sprintf(ptr+k, "0 ");

    // Controle de concorrencia
    //munlockall();

    //Distribuir os indices do vetor para cada filho
    for(int i=0; i<numProcess; i++){
        close(pipeToChild[i][PIPE_READ]);

        //enviar indice inicio
        buffer = i * (vtrSize / numProcess); 
        write(pipeToChild[i][PIPE_WRITE], &buffer, sizeof(buffer));

        //enviar indice final
        buffer = ((i+1) * (vtrSize / numProcess)) - 1; 
        if(i+1 == numProcess) buffer = vtrSize-1;
        write(pipeToChild[i][PIPE_WRITE], &buffer, sizeof(buffer));
    }

    //Receber o sinal de finalizacao de processo de cada filho
    for(int i=0; i<numProcess; i++){
        close(pipeToFather[i][PIPE_WRITE]);
        read(pipeToFather[i][PIPE_READ], &buffer, sizeof(buffer));
        if(buffer != 1) return printf("Pai: Filho %d falhou\n", i);
        printf("Pai: Filho %d retornou sinal de finalizacao\n", i);
    }

    //printf("pid %d, %s\n", pid, (char*)ptr);
    // Criar os 3 vetores a partir do shared memory
    int vtr[3][vtrSize];
    char x[10]; //buffer temporario
    k = 0;
    for(int i=0; i<3; i++){
        for(int j=0; j<vtrSize; j++){
            sscanf(ptr+k, "%s", x);
            k += strlen(x) + 1;
            vtr[i][j] = atoi(x);
        }
    }

    // resultados
    printf("Vetor com os Resultados: [");
    for(int i=0; i<vtrSize-1; i++){
        printf("%d, ", vtr[2][i]);
    }
    printf("%d]\n", vtr[2][vtrSize-1]);

    // unlink shared memory
    if(shm_unlink(memName) == -1) {
        printf("Error removing %s\n", memName);
        exit(-1);
    }

    wait(NULL);
    printf("Pai terminou de executar\n");
    return 0;
}