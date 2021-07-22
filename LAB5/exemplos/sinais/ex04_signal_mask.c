/**
 * Tutorial: https://github.com/angrave/SystemProgramming/wiki/
 * Signals%2C-Part-2%3A-Pending-Signals-and-Signal-Masks
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

sigset_t set;   //conjutno de sinais a serem bloqueados/mascarados

/* Define uma função tratadora de sinais. */
void sig_handler(int signo){
	printf("received a %d\n", signo);
}

int main(void){
    /* Associa a função tratadora de sinais */
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (signal(SIGQUIT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGQUIT\n");
    if (signal(SIGHUP, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGHUP\n");
    
    sigemptyset(&set);          //inicializa o conjunto com vazio
    //sigfillset(&set);         // adiciona todos os sinais
    sigaddset(&set, SIGQUIT);   // adiciona o sinal SIGQUIT
    sigaddset(&set, SIGINT);    // adiciona o sinal SIGINT
    sigprocmask(SIG_SETMASK, &set, NULL);   //aplica o mascaramento
    // SIGKILL e SIGSTOP não podem ser mascarados

    printf ("My PID is %d.\n", getpid());

    /* Entra em loop para pode dar tempo de receber sinais. */
    while(1)
       sleep(1);
    return 0;
}
