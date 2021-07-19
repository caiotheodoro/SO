/*
Descrição:
    Programa que lê a expressão escrita por P1 no FIFO, calcula a resposta para a mesma
    e a escreve em outro FIFO. Além de atuar como um 'servidor' que fica constantemente 
    recebendo "entradas" de P1.

Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    github.com/caiotheodoro/SO/commits/master
*/

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "/tmp/serverfifo"
#define SERVER_FIFO_2 "/tmp/serverfifo2"

int main (int argc, char **argv)
{
    int fd, fd2, bytes_read;
    char buf[512];

    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) 
    {
        perror ("mkfifo");
        exit (1);
    }

    // abre um FIFO
    if ((fd = open (SERVER_FIFO, O_RDONLY)) == -1)
        perror ("open");

    while (1) 
    {
        printf("z\n");
        // lê mensagem do FIFO
        memset (buf, '\0', sizeof(buf));
        if ((bytes_read = read(fd, buf, sizeof(buf))) == -1)
            perror ("read");
         
        if (bytes_read == 0) 
        {
            printf("-- None data...close and reopen fifo --\n");
            close(fd);
            fd = open (SERVER_FIFO, O_RDONLY);
            continue;
        }
        
        if (bytes_read > 0) 
        {
            int array_numbers[strlen(buf)];
            int result = 0;

            // salva todos numeros do buffer em um vetor
            for(int i = 0; i < strlen(buf); i++)
            {
                switch (buf[i])
                {
                    case '0':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '1':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '2':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '3':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '4':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '5':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '6':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '7':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '8':
                        array_numbers[i] = buf[i] - '0';
                        break;
                    
                    case '9':
                        array_numbers[i] = buf[i] - '0';
                        break;

                    default:
                        break;
                }
            }

            // realiza as contas
            for(int i = 0; i < strlen(buf); i++)
            {
                switch (buf[i])
                {
                    case '+':
                        array_numbers[i+1] = array_numbers[i-1] + array_numbers[i+1];
                        break;

                    case '-':
                        array_numbers[i+1] = array_numbers[i-1] - array_numbers[i+1];
                        break;

                    case '*':
                        array_numbers[i+1] = array_numbers[i-1] * array_numbers[i+1];
                        break;

                    case '/':
                        array_numbers[i+1] = array_numbers[i-1] / array_numbers[i+1];
                        break;
                    
                    default:
                        break;
                }
                result = (int) array_numbers[i];
            }
            buf[0] = result + '0';
            /*
            char* aux_buf[512];
            itoa(result, aux_buf, 10);
            */

            // retorna o resultado para P1 por meio de um segundo FIFO
            if ((mkfifo (SERVER_FIFO_2, 0664) == -1) && (errno != EEXIST)) 
            {
                perror ("mkfifo");
                exit (1);
            }

            fd2 = open(SERVER_FIFO_2, O_WRONLY);

            write(fd2, buf, strlen(buf));
            printf("Resultado gravado no fifo.\n");

            if(close(fd2) == -1)
                perror ("close");
        }       
    }

    if(close(fd) == -1)
        perror ("close");
}
