// Jogador, seu papel é escolher um animal que psoteriormente é recebido pelo mestre.
// um fifo é responsável por receber a resposta do server e o cliente responsável por mandar o dado para o server.

/*Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    https://github.com/caiotheodoro/SO/commits/master/LAB5/ex4 */
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

 #define SERVER_FIFO "/tmp/huya" //nome do server para comunicação

char fifo_f1 [128];
char buf1 [512], buf2 [1024];

int main (int argc, char **argv)
{
    int fd, fd_server, bytes_read;

    sprintf (fifo_f1, "/tmp/mestre%ld", (long) getpid ()); //path para criação + pid
    printf ("Bem vindo ao HUYA mestre!\n\n");
    printf("O HUYA consiste em cinco posições baseadas em cinco animais:\n O dragão, o coelho, a serpente, a águia e o tigre.\n\n");
    printf("O jogador escolhe um animal, e o mestre em um momento oportuno grita: HUYA!. Se o jogador estiver com a posição do mesmo animal que o mestre, o jogador perde.");
    //imprime explicação

    if (mkfifo (fifo_f1, 0664) == -1) //cria o fifo /tmp/mestre + pid
        perror ("mkfifo");

    while (1) {
        
             printf ("\n\nEscolha seu animal!\n1 - Dragão\n2 - Coelho\n3 - Serpente\n4 - Águia\n5 - Tigre.  : ");
        if (fgets(buf1, sizeof (buf1), stdin) == NULL) //capta animal digitado
            break;

        strcpy (buf2, fifo_f1);
        strcat (buf2, " ");
        strcat (buf2, buf1); //insere no buffer + path fifo

      

        if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) { //estabelece conexao coms erver
            perror ("abertura");
            break;
        }

        if (write (fd_server, buf2, strlen (buf2)) != strlen (buf2)) { //manda para o servidor
            perror ("escrita");
             break;
        }

        if (close (fd_server) == -1) {
            perror ("fechamento");
            break;
        }

       
        if ((fd = open (fifo_f1, O_RDONLY)) == -1) //abre arquivo fifo
           perror ("abertura");
        
        memset (buf2, '\0', sizeof (buf2)); //limpa buffer
        
        if ((bytes_read = read (fd, buf2, sizeof (buf2))) == -1) //le arquivo fifo
            perror ("leitura");

        if (bytes_read > 0) {
            printf ("\n\n %s\n", buf2); //imprime conteudo do arquivo fifo caso haja
        }

        if (close (fd) == -1) {
            perror ("fechamento");
            break;
        }
    }
    
    return 0;
}
