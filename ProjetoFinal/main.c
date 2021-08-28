#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commands.h"


#define BLOCKSIZE 4096
#define BLOCKQTDE 12

int main(int argc, char* argv[]){
    //CHECA SE EXISTE DISCO.TXT
    if(access(DISCO, F_OK) == 0){                   //arquivo existe
        printf("Disco carregado com sucesso!\n");
    }else{                                          //arquivo nao existe
        facc_format(BLOCKSIZE, BLOCKQTDE);
        printf("Disco criado.\n");
    }

    //SE EXISTIR, ELE IGNORA O FACC_FORMAT E CARREGA AS FUNCOES ABAIXO NORMALMENTE
    //SE NAO EXISTIR ELE PEDE NO BASH PRA FORMATAR

    //load superblock
    Superblock* sb = facc_loadSuperblock(BLOCKSIZE, BLOCKQTDE);
    printf("tipo %s, fatpos %d, rootpos %d, blocksize %d, blockQtde %d\n", sb->tipo, sb->fatPos, sb->rootPos, sb->blockSize, sb->blockQtde);
    
    //load fat
    Fat* fat = facc_loadFat(sb);
    printf("fat: ");
    for(int i=0; i<sb->blockQtde; i++){
        if      (fat[i] == FAT_L) printf("L, ");
        else if (fat[i] == FAT_F) printf("F, ");
        else if (fat[i] == FAT_R) printf("R, ");
        else if (fat[i] == FAT_B) printf("B, ");
        else                      printf("%d, ", fat[i]);
    }
    printf("\n");
    

    // READ_ROOT
    // DirChunk* diretorioAtual = facc_loadRoot(sb, fat);
 
    DirChunk* root = facc_loadRoot(sb, fat);

    printf("meta %s fblock %d, bytes %d, entryQtde %d\n", root->meta.name, root->meta.firstBlock, root->meta.bytes, root->meta.entryQtde);

    printf("entry1 %s type %s\n", root->entries[0]->name, root->entries[0]->type);
    printf("entry1 %s type %s\n", root->entries[1]->name, root->entries[1]->type);
    
    

    //  LS:
    //  entrar no dirchunk diretorioatual
    //  e listar o nome de cada struct entry do vetor de entries.

    //  CD:
    //  verificar no entry[] do diretorio atual, se o diretorio pretendido eh type="dir"
    //  se for type=dir, fazer igual a linha 43
    //  criar uma funcao baseada na facc_loadRoot(sb, fat);

    //  mkdir:
    //  criar o diretorio pretendido utilizando a createDirectory (provavel q tenha q mudar alguma coisa)
    //  criar uma entry no diretorioAtual com tipo dir referenciando o diretorio recem criado

    //  pwd:
    //  cria uma string pwd
    //  quando acessa qualquer diretorio vc adiciona /{nomeDir} (append)
    //  quando acessa o ".." voce remove o ultimo /{nomeDir}

    //  format dsc
    //  soh usar o facc_format, porem pedindo os parametros por um scanf




//     //WHILE TRUE COM O SHELL
//     int exit = 0;
//     char* option = malloc(sizeof(char)*256);
//     size_t optionSize = 256;

//     //arquivo existe
//     if(access(DISCO, F_OK) == 0){                   
//         while(1){
//             printf("Já existe um disco, deseja formata-lo? [y, n]\n");
//             // printf(" > y - manter\n > n - formatar\n");
//             getline(&option, &optionSize, stdin);

//             if(strncmp(option, "y", 1) == 0){
//                 //pedir blocksize e blockqtde
//                 facc_format(BLOCKSIZE, BLOCKQTDE);
//                 printf("Disco criado.\n");
//                 break;

//             }else if(strncmp(option, "n", 1) == 0) {
//                 printf("Disco carregado com sucesso!\n");
//                 break;

//             }else{
//                 printf("Opcao invalida.\n");
//             }  
//         }
        
//     //arquivo nao existe
//     }else{                                          
//         facc_format(BLOCKSIZE, BLOCKQTDE);
//         printf("Disco criado.\n");
//     }

//     // bash
//     while(exit == 0){

//         printf(">>");
//         getline(&option, &optionSize, stdin);
        
//         option = strtok(option, "\n"); //remove o \n

//         char* token;
//         char op[5][255];
// printf("teste\n");

//         token = strtok(option, " "); //pega a primeira palavra
//         strcpy(op[0], token);

//         printf("op0 %s\n", op[0]);
// printf("teste\n");
//         int i = 1;
//         while(token){
//             token = strtok(NULL, " ");
//              //pega a primeira palavra
//             strcpy(op[i], token);
//             i++;
//         }

//         printf("teste\n");

//         for(int j=i; j>=0; j--){
//             printf("op %d - %s\n", j, op[j]);
//         }


//         // printf("command %s // strlen %d \n", command, strlen(command));
        
//         // if(strcmp(option, "help") == 0){
//         //     help();
//         //     printf("entrou no help\n");
//         // }
//         // printf("oi %s strlen %d\n", option, strlen(option));

//         exit = 1;
//     }

//     free(option);
    free(fat);
    free(sb);
    return 1;
}

    // PEGAR COMANDO NO SHELL

    // pode ser util para adicionar arquivos externos ao disco.txt
    // access("arquivo.txt", 0);
        /* memset(input_str,0x0000,sizeof(input_str));
        printf(">> ");
        for (i = 0; (i < (sizeof(input_str)-1) &&
        ((ch = fgetc(stdin)) != EOF) && (ch != '\n')); i++){
            input_str[i] = ch;
        }
        input_str[i] = '\0'; */