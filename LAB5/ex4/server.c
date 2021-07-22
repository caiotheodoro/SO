 

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/addition_fifo_server"
#define MAX_NUMBERS 500

int main (int argc, char **argv)
{
    int fd, fd_client, bytes_read, i;
    char buf [4096];

    char *return_fifo;
    char *numbers [MAX_NUMBERS];


    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) {
        perror ("mkfifo");
        exit (1);
    }
    if ((fd = open (SERVER_FIFO, O_RDONLY)) == -1)
        perror ("open");



     while (1) {
         // get a message
 
         memset (buf, '\0', sizeof (buf));
         if ((bytes_read = read (fd, buf, sizeof (buf))) == -1)
             perror ("read");
         if (bytes_read == 0)
             continue;


		

         if (bytes_read > 0) {
             return_fifo = strtok (buf, ", \n");
            printf("%d", bytes_read);
             i = 0;
             numbers [i] = strtok (NULL, ", \n");

             while (numbers [i] != NULL && i < MAX_NUMBERS)
                 numbers [++i] = strtok (NULL, ", \n");

             int total_numbers = i;
             double valServer = rand() % 5;
             unsigned int error = 0;
             char *ptr;
            double value;
            char* animal;
             for (i = 0; i < total_numbers; i++) {
                 double f = strtod (numbers [i], &ptr);

                 if (*ptr) {
                     error = 1;
                     break;
                 }
                 value = f;
             }
            int conv = (int) valServer;
            switch(conv){
                case 1:
                animal = "Dragão";
                break;
                case 2:
                 animal = "Coelho";
                break;
                case 3:
                 animal = "Serpente";
                break;
                case 4:
                 animal = "Àguia";
                break;
                case 5:
                 animal = "Tigre";
                break;
                default:
                break;
            }
           
             if ((fd_client = open (return_fifo, O_WRONLY)) == -1) {
                 perror ("open: client fifo");
                 continue;
             }

             if (error)
                 sprintf (buf, "Error in input.\n");
            else if(valServer == value){
                sprintf (buf, "Perdeu! server escolheu %s\n", animal);
                    
                }
            else {
                sprintf (buf, "Ganhou! server escolheu %s\n", animal);
            }
               
             if (write (fd_client, buf, strlen(buf)) != strlen(buf))
                 perror ("write");

             if (close (fd_client) == -1)
                 perror ("close");
         }
        
        
     }
}
