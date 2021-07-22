 
/*
     write messages to FIFO

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
#define N 10

char buf [512];

int main (int argc, char **argv)
{
    int fd_server;

    if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) {
        perror ("open: server fifo");
        return 1;
    }

    int value = 0;
    while (value < N) {
        // cria uma mensagem
        value++;
        sprintf (buf, "msg %ld - number %d", (long) getpid (), value);
        printf("Sending: %s\n", buf);

        // envia mensagem para o fifo
        write (fd_server, buf, strlen (buf));
    
        //sleep(1);
    }
        
   if (close (fd_server) == -1) {
       perror ("close");
       return 2;
   }

          
    return 0;
}
