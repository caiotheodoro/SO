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
#include "commands.h"
#include "fatacc.h"

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





char** split(char* command)
{
    char** result    = 0;
    size_t count     = 0;
    char delimiter = ' ';
    char* tmp  = command;
    char* last_comma = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (command + strlen(command) - 1);
    count++;
    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(command, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void listenCommand(char* command,DirChunk* diretorioAtual,Superblock* sb, int blockNum){

    char** vals = split(command);
    char* type = *(vals + 0);
    char* source = *(vals + 1);
    char* destination = *(vals + 2);
    

    if(strcmp(type,"cd") == 0){
        changeDirectory(diretorioAtual,sb,source,blockNum);
    }else if(strcmp(type,"mkdir") == 0){
        makeDirectory(diretorioAtual,sb,source,blockNum);
    }else if(strcmp(type,"rm") == 0){
        rmItem(diretorioAtual,sb,source);
    }else if(strcmp(type,"cp") == 0){
        copyItem(diretorioAtual,sb,source,destination);
    }else if(strcmp(type,"mv") == 0){
        moveItem(diretorioAtual,sb,source,destination);
    }else if(strcmp(type,"ls") == 0){
        listDirectory(diretorioAtual,sb);
    }else if(strcmp(type,"pwd") == 0){
        showPath(diretorioAtual,sb);
    }

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

//dirMeta (?)
}
void copyItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
//dirMeta (?)
}
void moveItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){

}
void listDirectory(DirChunk* diretorioAtual,Superblock* sb){

}
void showPath(DirChunk* diretorioAtual,Superblock* sb){

}
