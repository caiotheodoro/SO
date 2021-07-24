 // servidor para a resposta das jogadas. recebe os dois animais enviados
 //e imprime a resposta para o mestre. 

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define SERVER_FIFO "/tmp/huya"//nome do server para comunicação
#define MAX_NUMBERS 2 

int main (int argc, char **argv)
{
    int fd, fd_client, bytes_read, i;
    char buf [4096];
    int count=0;
    double valJogador;
    char *ptr;
    char *return_fifo;
    char *numbers [MAX_NUMBERS];


    if ((mkfifo (SERVER_FIFO, 0664) == -1) && (errno != EEXIST)) {//verifica conexao com o server_fifo
        perror ("mkfifo");
        exit (1);
    }
    if ((fd = open (SERVER_FIFO, O_RDONLY)) == -1) //abre o server
        perror ("open");

     while (1) {
    
 
         memset (buf, '\0', sizeof (buf)); //limpa o buffer
         if ((bytes_read = read (fd, buf, sizeof (buf))) == -1) //le o server e verifica se há conteudo para a leitura
             perror ("read");
         if (bytes_read == 0)
             continue;
             
            
         if (bytes_read > 0) { //se há conteudo
             count++; //conta (primeiro processo se comunidando)
             return_fifo = strtok (buf, ", \n");
            
             i = 0;
             numbers [i] = strtok (NULL, ", \n"); 

             while (numbers [i] != NULL && i < MAX_NUMBERS)
                 numbers [++i] = strtok (NULL, ", \n");

             int total_numbers = i;
             if(count ==1){
             valJogador = strtod(numbers[0],&ptr); //armazena primeiro valor mandado pelo processo
             }
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
            int conv = (int) valJogador; //usado no switch para determinar string do animal enviado
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
            if(count == 2){ //quando for 2, compara os animais mandados
           
           
             if ((fd_client = open (return_fifo, O_WRONLY)) == -1) {
                 perror ("open: client fifo");
                 continue;
             }

             if (error)
                 sprintf (buf, "Error in input.\n");
            else if(valJogador == value){ //se igual, jogador perdeu.
                sprintf (buf, "Mestre ganhou! jogador escolheu %s e perdeu.\n", animal);
                    
                }
            else { //se não, jogador ganhou
                sprintf (buf, "Mestre perdeu! jogador escolheu %s\n e ganhou", animal);
            }
               
             if (write (fd_client, buf, strlen(buf)) != strlen(buf)) //escreve no arquivo fifo
                 perror ("write");

             if (close (fd_client) == -1)
                 perror ("close");
                 
                count=0;
         }
         else { // se não recebeu ainda primeiro animal, imprime jogada feita
             if ((fd_client = open (return_fifo, O_WRONLY)) == -1) {
                 perror ("open: client fifo");
                 continue;
             }

             if (error)
                 sprintf (buf, "Error in input.\n");
            else {
                sprintf (buf, "Jogada feita! vez do mestre...");
            
            }
               
             if (write (fd_client, buf, strlen(buf)) != strlen(buf)) //escreve no arquivo fifo
                 perror ("write");

            
             if (close (fd_client) == -1)
                 perror ("close");
         }
         }
        
     }
}
