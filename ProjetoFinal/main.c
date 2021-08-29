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
void listenCommand(Superblock** sb, Fat** fat, DirChunk** diretorioAtual, char* command,char *pathing);

int main(int argc, char* argv[]){
    char command[250];
    char* pathing = createPathing();

    //arquivo existe
    if(access(DISCO, F_OK) == 0){                   
        while(1){
            printf("Já existe um disco, deseja formata-lo? [y, n]\n");
            printf(" > y - manter\n > n - formatar\n");
            fgets(command, sizeof(command), stdin); 

            if(strncmp(command, "y", 1) == 0){
                //pedir blocksize e blockqtde
                printf("Quantidade de blocos: ");
                fgets(command, sizeof(command), stdin);
                format_dsc(atoi(command));
                printf("Disco criado.\n");
                break;

            }else if(strncmp(command, "n", 1) == 0) {
                printf("Disco carregado com sucesso!\n");
                break;

            }else{
                printf("Opcao invalida.\n");
            }  
        }
        
    //arquivo nao existe
    }else{                                          
        facc_format(BLOCKSIZE, BLOCKQTDE);
        printf("Disco gerado com &d blocos de %d bytes.\nUtilize o comando format dsc <qtdeBlocos> para formatar", BLOCKQTDE, BLOCKSIZE);
    }
    //carrega na memoria
    //load superblock
    Superblock* sb = facc_loadSuperblock();
    
    //load fat
    Fat* fat = facc_loadFat(sb);

    // READ_ROOT
    DirChunk* diretorioAtual = facc_loadDir(sb, fat, sb->rootPos);


    //SHELL
    while(1){
        printf("(%s)$ > ", pathing);
        fgets(command, sizeof(command), stdin);
        if(strncmp(command, "exit", 4) == 0) break;
        listenCommand(&sb, &fat, &diretorioAtual, command, pathing);
    }

    free(sb);
    free(fat);
    facc_unloadDirectory(diretorioAtual);
    return 0;
}

// int main(int argc, char* argv[]){
//     //CHECA SE EXISTE DISCO.TXT
//     if(access(DISCO, F_OK) == 0){                   //arquivo existe
//         printf("Disco carregado com sucesso!\n");
//     }else{                                          //arquivo nao existe
//         facc_format(BLOCKSIZE, BLOCKQTDE);
//         printf("Disco criado.\n");
//     }

/*
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
*/

void listenCommand(Superblock** sb, Fat** fat, DirChunk** diretorioAtual, char* command,char* pathing){
    //pegar a linha de comando e dividir em palavras
    char** vals = split(command);
    char *cmd, *source, *destination;
    int cmdCount = 0;

    if(*(vals + 0) != NULL){
        cmd = *(vals + 0);
        cmdCount++;
    }
    if(*(vals + 1) != NULL){
        source = *(vals + 1);
        cmdCount++;
    }
    if(*(vals + 2) != NULL){
        destination = *(vals + 2);
        cmdCount++;
    }


    if(strcmp(cmd, "help") == 0){
        help();

    }else if(strcmp(cmd,"cd") == 0){
        if(!source || cmdCount < 2){
            printf("cd <dirName>\n");
            return;
        }
        *diretorioAtual = changeDirectory(*sb, *fat, *diretorioAtual, source, pathing);
        
    }else if(strcmp(cmd,"mkdir") == 0){
        if(!source || cmdCount < 2){
            printf("mkdir <dirName>\n");
            return;
        }
        makeDirectory(*sb, *fat, *diretorioAtual, source);
    
    }else if(strcmp(cmd,"rm") == 0){
        if(!source || cmdCount < 2){
            printf("mkdir <dirName>\n");
            return;
        }
        removeItem(*sb, *fat, *diretorioAtual, source);

    // }else if(strcmp(type,"cp") == 0){
    //     copyItem(*diretorioAtual, *sb,source, destination);

    // }else if(strcmp(type,"mv") == 0){
    //     moveItem(*diretorioAtual, *sb,source, destination);

    }else if(strcmp(cmd, "ls") == 0){
        listDirectory(*diretorioAtual, source);

    }else if(strcmp(cmd, "pwd") == 0){
        showPath(pathing);

    }else if(strcmp(cmd, "format") == 0 && strcmp(source, "dsc") == 0){
        format_dsc(atoi(destination));
            
        //atualiza a memoria
        //load superblock
        free(*sb);
        *sb = facc_loadSuperblock();
        
        //load fat
        free(*fat);
        *fat = facc_loadFat(*sb);
        
        // READ_ROOT
        facc_unloadDirectory(*diretorioAtual);
        *diretorioAtual = facc_loadDir(*sb, *fat, (*sb)->rootPos);

    }else{
        printf("Comando '%s' não encontrado, digite 'help' para ver os comandos disponíveis.\n", cmd);
    }

}

char** split(char* command){

    command = strtok(command, "\n");

    char** result    = 0;
    size_t count     = 0;
    char* tmp  = command;
    char* last_comma = 0;

    char delimiter = ' ';
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp){
        if (delimiter == *tmp){
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (command + strlen(command) - 1);
    count++;
    result = malloc(sizeof(char*) * count);

    if (result){
        size_t idx  = 0;
        char* token = strtok(command, delim);

        while (token){
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }

        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}