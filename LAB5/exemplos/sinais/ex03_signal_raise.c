/*
 * Fonte: http://www.gnu.org/software/libc/manual/html_node/
 *        Signaling-Yourself.html#Signaling-Yourself
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/* Define uma função tratadora de sinais. */
void sig_handler(int signo){
    if (signo == SIGTERM){
        printf("received SIGTERM\n");
        printf("Eu deveria ter finalizado...\n");
    }
    
    if (signo == SIGALRM){
        printf("received SIGALRM\n");
        //raise(SIGKILL);
        kill(getpid(), SIGKILL);
    }
}

int main(void){

    /* Associa a função tratadora de sinais */
    if (signal(SIGTERM, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGTERM\n");

    if (signal(SIGALRM, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGALRM\n");

    alarm(10);

    /* exibe o PID */
    printf("My PID is %d.\n", getpid());
  
    /* Entra em loop para pode dar tempo de receber sinais. */
    while(1)
        sleep(1);
  
    return 0;
}


