/*
Descrição:
    Programa que recebe uma frase através de um arquivo "decodificado.txt" e a traduz para codigo morse.
    Com a implementação adicional do graceful stop ao receber os sinais SIGINT ou SIGTERM.

Autores:
    Caio Theodoro, Caio Miglioli, Alexandre Scrocaro

Datas:
    github.com/caiotheodoro/SO/commits/master
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t keep_going = 1; 

void write_file(char* saida[], int n)
{
    FILE *p;
    p = fopen("codificado.txt", "w");
    fwrite(saida, n, 1, p);
    fclose(p);
}

/* função tratadora de sinais. */
void sig_handler(int signo){//, char* saida[], int n
	if (signo == SIGINT)
		printf("\nreceived SIGINT\n");

    if (signo == SIGTERM){
        printf("\nreceived SIGTERM\n");
        
    }
    printf("Graceful stop\n");
    keep_going = 0;
}

int main(void){
    /* Associa a função tratadora de sinais */
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGTERM\n");

    /* exibe o PID */
    printf("My PID is %d.\n", getpid());


    /* -------------------- CÓDIGO QUE MANIPULA ARQUIVOS -------------------- */
    // Abre um .txt com intuito transformar uma frase normal em código morse
    // salva o código morse gerado em outro .txt
    // coloquei alguns sleep(1) no meio do código para conseguir pará-lo a qualquer momento


    int i;
    char entrada[1024], saida[1024] = "";

    FILE *arq;
    arq = fopen("decodificado.txt", "r");
    fscanf(arq, " %[^\n]", entrada);
    fclose(arq);

    for(i=0; i<strlen(entrada);)
    {
        // laço para implementar o graceful stop
        while(keep_going)
        {
            if(entrada[i] == 'a' || entrada[i] == 'A')	     strcat(saida, ".- ");
            else if(entrada[i] == 'b' || entrada[i] == 'B')  strcat(saida, "-... ");
            else if(entrada[i] == 'c' || entrada[i] == 'C')  strcat(saida, "-.-. ");
            else if(entrada[i] == 'd' || entrada[i] == 'D')  strcat(saida, "-.. ");
            else if(entrada[i] == 'e' || entrada[i] == 'E')  strcat(saida, ". ");
            else if(entrada[i] == 'f' || entrada[i] == 'F')  strcat(saida, "..-. ");
            else if(entrada[i] == 'g' || entrada[i] == 'G')  strcat(saida, "--. ");
            else if(entrada[i] == 'h' || entrada[i] == 'H')  strcat(saida, ".... ");
            else if(entrada[i] == 'i' || entrada[i] == 'I')  strcat(saida, ".. ");
            else if(entrada[i] == 'j' || entrada[i] == 'J')  strcat(saida, ".--- ");
            else if(entrada[i] == 'k' || entrada[i] == 'K')  strcat(saida, "-.- ");
            else if(entrada[i] == 'l' || entrada[i] == 'L')  strcat(saida, ".-.. ");
            else if(entrada[i] == 'm' || entrada[i] == 'M')  strcat(saida, "-- ");
            else if(entrada[i] == 'n' || entrada[i] == 'N')  strcat(saida, "-. ");
            else if(entrada[i] == 'o' || entrada[i] == 'O')  strcat(saida, "--- ");
            else if(entrada[i] == 'p' || entrada[i] == 'P')  strcat(saida, ".--. ");
            else if(entrada[i] == 'q' || entrada[i] == 'Q')  strcat(saida, "--.- ");
            else if(entrada[i] == 'r' || entrada[i] == 'R')  strcat(saida, ".-. ");
            else if(entrada[i] == 's' || entrada[i] == 'S')  strcat(saida, "... ");
            else if(entrada[i] == 't' || entrada[i] == 'T')  strcat(saida, "- ");
            else if(entrada[i] == 'u' || entrada[i] == 'U')  strcat(saida, "..- ");
            else if(entrada[i] == 'v' || entrada[i] == 'V')  strcat(saida, "...- ");
            else if(entrada[i] == 'w' || entrada[i] == 'W')  strcat(saida, ".-- ");
            else if(entrada[i] == 'x' || entrada[i] == 'X')  strcat(saida, "-..- ");
            else if(entrada[i] == 'y' || entrada[i] == 'Y')  strcat(saida, "-.-- ");
            else if(entrada[i] == 'z' || entrada[i] == 'Z')  strcat(saida, "--.. ");
            else if(entrada[i] == ' ')				     strcat(saida, " \\ ");
            else if(entrada[i] == '1') 				     strcat(saida, ".---- ");
            else if(entrada[i] == '2')                   strcat(saida, "..--- ");
            else if(entrada[i] == '3')				     strcat(saida, "...-- ");
            else if(entrada[i] == '4')				     strcat(saida, "....- ");
            else if(entrada[i] == '5')				     strcat(saida, "..... ");
            else if(entrada[i] == '6')                   strcat(saida, "-.... ");
            else if(entrada[i] == '7')                   strcat(saida, "--... ");
            else if(entrada[i] == '8')                   strcat(saida, "---.. ");
            else if(entrada[i] == '9')                   strcat(saida, "----. ");
            else if(entrada[i] == '0')                   strcat(saida, "----- ");
            
            // printf("saida tmp: %s\n", saida);
            i++;
            sleep(1);

            if(i == strlen(entrada))
                keep_going = 0;
        }

        break;
    }

    printf("Mensagem decodificada: %s\n", entrada);
    printf("Mensagem codificada obtida: %s\n", saida);
    

    write_file(saida, strlen(saida));

    kill(getpid(), SIGKILL);
}
