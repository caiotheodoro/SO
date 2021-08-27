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

// teste 