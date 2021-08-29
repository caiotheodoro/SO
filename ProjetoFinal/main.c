#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

// #include "fatacc.h"
#include "commands.h"

#define BLOCKSIZE 4096
#define BLOCKQTDE 16

char** split(char* command);
void listenCommand(Superblock** sb, Fat** fat, DirChunk** diretorioAtual, char* command,char *pathing); // Listener

int main(int argc, char* argv[]){
    char command[250];
    char* pathing = createPathing(); // Cria o caminho atual (root)

    //arquivo existe
    if(access(DISCO, F_OK) == 0){                   // Verifica se o arquivo existe
        while(1){                                // Enquanto o usuário não digitar "exit"
            printf("Já existe um disco, deseja formata-lo? [y, n]\n"); // Pergunta se deseja formatar o disco
            fgets(command, sizeof(command), stdin);            // Recebe a resposta do usuário

            if(strncmp(command, "y", 1) == 0){               // Se a resposta for "y"
                //pedir blocksize e blockqtde
                printf("Quantidade de blocos: ");            // Pergunta quantos blocos o disco deve ter
                fgets(command, sizeof(command), stdin);     // Recebe a resposta do usuário
                format_dsc(atoi(command));                  // Chama a função que formata o disco
                printf("Disco criado.\n");                 // Informa que o disco foi criado
                break;                                     // Sai do loop

            }else if(strncmp(command, "n", 1) == 0) {     // Se a resposta for "n"
                printf("Disco carregado com sucesso!\n"); // Informa que o disco foi carregado
                break;

            }else{
                printf("Opcao invalida.\n");
            }  
        }
        
    //arquivo nao existe
    }else{                                          
        facc_format(BLOCKSIZE, BLOCKQTDE);           // Chama a função que cria o disco
        printf("Disco gerado com &d blocos de %d bytes.\nUtilize o comando format dsc <qtdeBlocos> para formatar", BLOCKQTDE, BLOCKSIZE);   // Informa que o disco foi criado
    }
    //carrega na memoria
    //load superblock
    Superblock* sb = facc_loadSuperblock();         // Carrega o superblock na memória
    
    //load fat
    Fat* fat = facc_loadFat(sb);                   // Carrega o fat na memória

    // READ_ROOT 
    DirChunk* diretorioAtual = facc_loadDir(sb, fat, sb->rootPos); // Carrega o diretório raiz na memória


    //SHELL
    while(1){
        printf("(%s)$ > ", pathing);           // Imprime o caminho atual
        fgets(command, sizeof(command), stdin); // Recebe a entrada do usuário
        if(strncmp(command, "exit", 4) == 0) break; // Se a entrada for "exit"
        listenCommand(&sb, &fat, &diretorioAtual, command, pathing); // Chama a função que escuta a entrada do usuário
    }

    free(sb); // Libera o espaço alocado para o superblock
    free(fat); // Libera o espaço alocado para o fat
    facc_unloadDirectory(diretorioAtual); // Libera o espaço alocado para o diretório raiz
    return 0; 
}

/*
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
*/

void listenCommand(Superblock** sb, Fat** fat, DirChunk** diretorioAtual, char* command,char* pathing){
    //pegar a linha de comando e dividir em palavras
    char** vals = split(command); //cria um vetor de strings com as palavras da linha de comando
    char *cmd, *source, *destination; //cria variaveis para guardar os valores de cada palavra
    int cmdCount = 0; //contador de palavras
    if(*(vals + 0) != NULL){ //se a primeira palavra nao for nula
        cmd = *(vals + 0); //
        cmdCount++;
    }
    if(*(vals + 1) != NULL){ //se a segunda palavra nao for nula
        source = *(vals + 1); //pega o valor da segunda palavra
        cmdCount++;
    }
    if(*(vals + 2) != NULL){ //se a terceira palavra nao for nula
        destination = *(vals + 2); //pega o valor da terceira palavra
        cmdCount++;
    }
    if(strcmp(cmd, "help") == 0){ //se a primeira palavra for help
        help();

    }else if(strcmp(cmd,"cd") == 0){ //se a primeira palavra for cd
        if(!source || cmdCount < 2){
            printf("cd <dirName>\n");
            return;
        }
        *diretorioAtual = changeDirectory(*sb, *fat, *diretorioAtual, source, pathing); //muda o diretorio atual
    }else if(strcmp(cmd,"mkdir") == 0){ //se a primeira palavra for mkdir
        if(!source || cmdCount < 2){ //se nao tiver o nome do diretorio
            printf("mkdir <dirName>\n"); 
            return;
        }
        makeDirectory(*sb, *fat, *diretorioAtual, source); //cria o diretorio
    }else if(strcmp(cmd,"rm") == 0){ //se a primeira palavra for rm
        if(!source || cmdCount < 2){ //se nao tiver o nome do item (diretorio ou arquivo)
            printf("mkdir <dirName>\n");
            return;
        }
        removeItem(*sb, *fat, *diretorioAtual, source);      
    }else if(strcmp(cmd,"cp") == 0){
         copyItem(*diretorioAtual, *sb,source, destination);

     }else if(strcmp(cmd,"mv") == 0){ //se a primeira palavra for mv
         moveItem(*diretorioAtual, *sb,source, destination); //move o item (diretorio ou arquivo)

    }else if(strcmp(cmd, "ls") == 0){ //se a primeira palavra for ls
        listDirectory(*diretorioAtual, source); //lista o diretorio atual

    }else if(strcmp(cmd, "pwd") == 0){ //se a primeira palavra for pwd
        showPath(pathing); //mostra o caminho atual

    }else if(strcmp(cmd, "format") == 0 && strcmp(source, "dsc") == 0){ //se a primeira palavra for format e o segundo for dsc
        format_dsc(atoi(destination)); //formata o disco com a quantidade de blocos especificada
            
        //atualiza a memoria
        //load superblock
        free(*sb); //libera a memoria do superblock
        *sb = facc_loadSuperblock(); //carrega o superblock
        
        //load fat
        free(*fat); //libera a memoria do fat
        *fat = facc_loadFat(*sb); //carrega o fat
        
        // READ_ROOT
        facc_unloadDirectory(*diretorioAtual); //libera a memoria do diretorio atual
        *diretorioAtual = facc_loadDir(*sb, *fat, (*sb)->rootPos); //carrega o diretorio raiz

    }else{ //se nao for nenhuma das opcoes
        printf("Comando '%s' não encontrado, digite 'help' para ver os comandos disponíveis.\n", cmd);
    }
}

char** split(char* command){

    command = strtok(command, "\n"); //remove o \n do final da string

    char** result    = 0; //ponteiro para o resultado (string como array de char)
    size_t count     = 0; //contador de palavras
    char* tmp  = command; //ponteiro temporario para percorrer a string
    char* last_comma = 0;
    char delimiter = ' ', delim[2]; //delimitador
    delim[0] = delimiter, delim[1] = 0; 

    while (*tmp){ //enquanto nao chegar no fim da string
        if (delimiter == *tmp){ //se encontrar um delimitador
            count++; //incrementa o contador
            last_comma = tmp; //salva o ponteiro para o ultimo espaço encontrado
        }
        tmp++; //incrementa o ponteiro
    }
    count += last_comma < (command + strlen(command) - 1); //se o ultimo espaço nao for
                                                           // o fim da string, incrementa o contador
    count++;
    result = malloc(sizeof(char*) * count); //aloca o tamanho do contador
    if (result){ //se a alocacao foi bem sucedida
        size_t idx  = 0; //indice do resultado
        char* token = strtok(command, delim); //pega a primeira palavra

        while (token){ //enquanto nao chegar no fim da string
            assert(idx < count); //se o indice for maior que o contador, algo de errado aconteceu
            *(result + idx++) = strdup(token); //copia a palavra para o resultado
            token = strtok(0, delim); //pega a proxima palavra
        }
        assert(idx == count - 1); //se o indice for diferente do contador, algo de errado aconteceu
        *(result + idx) = 0; //faz o fim da string
    }
    
    return result; //retorna o resultado
}
