/*
 * Utilizar o kill -SIGINT pid
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/* função tratadora de sinais. */
void sig_handler(int signo){
	if (signo == SIGINT)
		printf("received SIGINT\n");
}

int main(void){
    /* Associa a função tratadora de sinais */
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    /* exibe o PID */
    printf("My PID is %d.\n", getpid());

    /* Simulando uma execução de nada */
    while(1)
        sleep(1);
    
    return 0;
}
