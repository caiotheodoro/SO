 
/*
     add_client.c: Client program to take user input comprising
                   of numbers, send request message to add_server
                   and receive server's response

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

#define SERVER_FIFO "/tmp/addition_fifo_server"

char my_fifo_name [128];
char buf1 [512], buf2 [1024];

int main (int argc, char **argv)
{
    int fd, fd_server, bytes_read;

    sprintf (my_fifo_name, "/tmp/jogador%ld", (long) getpid ());
    printf ("Bem vindo ao HUYA jogador!\n\n");
    printf("O HUYA consiste em cinco posições baseadas em cinco animais:\n O dragão, o coelho, a serpente, a águia e o tigre.\n\n");
    printf("O jogador escolhe um animal, e o mestre em um momento oportuno grita: HUYA!. Se o jogador estiver com a posição do mesmo animal que o mestre, o jogador perde.");

    if (mkfifo (my_fifo_name, 0664) == -1)
        perror ("mkfifo");

    while (1) {
    
             printf ("\n\nEscolha seu animal!\n1 - Dragão\n2 - Coelho\n3 - Serpente\n4 - Águia\n5 - Tigre.  : ");
        if (fgets(buf1, sizeof (buf1), stdin) == NULL)
            break;

        strcpy (buf2, my_fifo_name);
        strcat (buf2, " ");
        strcat (buf2, buf1);



        if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) {
            perror ("open: server fifo");
            break;
        }

        if (write (fd_server, buf2, strlen (buf2)) != strlen (buf2)) {
            perror ("write");
             break;
        }

        if (close (fd_server) == -1) {
            perror ("close");
            break;
        }

        
        if ((fd = open (my_fifo_name, O_RDONLY)) == -1)
           perror ("open");
        
        memset (buf2, '\0', sizeof (buf2));
        
        if ((bytes_read = read (fd, buf2, sizeof (buf2))) == -1)
            perror ("read");

        if (bytes_read > 0) {
            printf ("\n\n\n%s\n", buf2);
        }

        if (close (fd) == -1) {
            perror ("close");
            break;
        }
    }
    
    return 0;
}
