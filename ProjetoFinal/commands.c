/*
Pode ser util para o comando "mv"
    #include <fcntl.h>
  > int open(const char *path, int oflag, ... );
    https://pubs.opengroup.org/onlinepubs/009695399/functions/open.html

    The read() function shall attempt to read nbyte bytes from the file associated with the open file descriptor, fildes, into the buffer pointed to by buf. 
    #include <unistd.h>
  > ssize_t read(int fildes, void *buf, size_t nbyte);
    [XSI] [Option Start] ssize_t pread(int fildes, void *buf, size_t nbyte, off_t offset); [Option End]
    https://pubs.opengroup.org/onlinepubs/009695399/functions/read.html

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "commands.h"

void help(){
    printf("Descricao dos comandos:\n\n");
 
    printf("cd - Acessa um diretorio\n");
    printf("\tcd <dir>\n\tcd /caminho/nome\n\n");

    printf("mkdir - Cria um diretorio\n");
    printf("\tmkdir <dir>\n\tmkdir /caminho/nome\n\n");

    printf("rm - Remove um diretorio/arquivo\n");
    printf("\trm <dir | arquivo>\n\trm /caminho/nome\n\n");

    printf("cp - Copia um diretorio/arquivo\n");
    printf("\tcp <dir | arquivo>\n\tcp /caminho/nome\n\n");

    printf("mv - Move Diretório/Arquivo.\n");
    printf("\tmv <dir | arquivo>\n\n");

    printf("ls - Lista o conteudo de um diretorio ou mostra as informacoes ddo arquivo\n");
    printf("\tls <dir | arquivo>\n\n");

    printf("pwd - Mostra o caminho do diretorio atual\n");
    printf("\tpwd\n\n");

    printf("format dsc - Formata o disco simulado\n");
    printf("\tformat dsc\n\n");
}




void changeDirectory(DirChunk* diretorioAtual,Superblock* sb,char* name, int blockNum){
  if(blockNum >= sb->blockQtde) return;
  //dirMeta (?)

}

void makeDirectory(DirChunk* diretorioAtual,Superblock* sb,char* name, int blockNum){
  if(blockNum >= sb->blockQtde) return;
  //dirMeta (?)

}

void rmItem(DirChunk* diretorioAtual,Superblock* sb,char* name){
    printf("rmItem");
//dirMeta (?)
}

void copyItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
    printf("copyItem");
//dirMeta (?)
}

void moveItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
    printf("moveItem");
}

void listDirectory(DirChunk* diretorioAtual, char* name){
    //se nao ha segundo parametro
    if(name == NULL){
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            printf("%s\t", diretorioAtual->entries[i]->name);
        }
    
    //se o ultimo elemento for * mostra todos os arquivos com as iniciais
    }else if(name[strlen(name)-1] == '*') {
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strncmp(diretorioAtual->entries[i]->name, name, strlen(name)-1) == 0){
                printf("%s\t", diretorioAtual->entries[i]->name);
            }
        }
    
    //se esta procurando um arquivo especifico
    }else{
        int notFound = 1;
        
        //se name = ..DOIS/musicas/eletronica
        //char aux; e passo pro aux tudo antes do / e retiro essa palavra do name
        //aux = ..DOIS
        //name = musicas/eletronica

        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){         
            //if(strcmp(diretorioAtual->entries[i]->name, aux) == 0){
            //fazer a comparacao do entry[i] com o aux
            //verificar se name esta vazio, se estiver eh pq esta eh a ultima recursao
            //se for a ultima recursao, eu mostro todos os itens do diretorio
            //se nao for a ultima recursao, eu chamo o openDir com o diretorio encontrado
            //e chamo listDirectory com name = name
            
            if(strcmp(diretorioAtual->entries[i]->name, name) == 0){
                //se encontrou o arquivo, checar se eh dir
                if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                    printf("eh diretorio\n");
                    printf("%s\t", diretorioAtual->entries[i]->name);
                    //chamar open dir e listar os diretorios dentro do diretorio "name"
                }else{
                    printf("%s\t", diretorioAtual->entries[i]->name);
                }
                notFound = 0;
            }
        }
        
        if(notFound == 1){
            printf("Arquivo ou Diretorio inexistente.");
        }
    }
    printf("\n");
}


void showPath(DirChunk* diretorioAtual,Superblock* sb){
    printf("showPath");

}

