/*
Descrição:
    Programa que recebe uma expressão matemática (como: 2+2) e a escreve em um FIFO,
    para que P2 possa ler. Além de receber a resposta de P1,por meio de outro FIFO,
    e a exibir na tela

Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    https://github.com/caiotheodoro/SO/commits/master/LAB5/ex1
*/

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>

#define SERVER_FIFO "/tmp/serverfifo"
#define SERVER_FIFO_2 "/tmp/serverfifo2"

int main (int argc, char **argv)
{
    int fd_server, fd_server2, bytes_read;
    char buf2[512];

    // cria um FIFO se inexistente
    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) 
    {
        perror ("mkfifo");
        exit (1);
    }
    
    if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) {
        perror ("open: server fifo");
        return 1;
    }

    // salva a expressão matemática no buffer
    char* buf = argv[1];
    
    // envia mensagem para o fifo
    write(fd_server, buf, strlen(buf));
    
    if (close (fd_server) == -1) {
        perror ("close");
        return 2;
    }

    // exibe resultado na tela 
    if ((mkfifo (SERVER_FIFO_2, 0664) == -1) && (errno != EEXIST)) 
    {
        perror ("mkfifo");
        exit (1);
    }

    if((fd_server2 = open(SERVER_FIFO_2, O_RDONLY)) == -1) {
        perror ("open: server fifo");
        return 1;
    }

    memset(buf2, '\0', sizeof(buf2));
    if ((bytes_read = read(fd_server2, buf2, sizeof(buf2))) == -1)
            perror("read");

    printf("Result:  %c\n", buf2[0]);

    if (close (fd_server) == -1) {
        perror ("close");
        return 2;
    }
    

    return 0;
}
