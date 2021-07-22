/*
 * Exemplo: http://www.gnu.org/software/libc/manual/
 *               html_node/Handler-Returns.html#Handler-Returns
 */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Flag que controla a terminação do loop. */
volatile sig_atomic_t keep_going = 1;

/* Tratador para o sinal SIGALRM. Reseta o flag e se reabilita. */
void catch_alarm(int sig) {
    puts("Alarme!");
    keep_going = 0;
    signal(sig, catch_alarm);
}

void do_stuff(void) {
    puts("Fazendo alguma coisa enquanto aguarda o alarme.");
    sleep(1);
}

int main(void) {
    /* Estabelece um tratador para sinais SIGALRM. */
    signal(SIGALRM, catch_alarm);

    /* Define um alarme para daqui a 10 segundos.
     * Interromperá o laço. */
    alarm(10);

    /* Fica em loop executando. */
    while (keep_going)
        do_stuff();

    puts("Terminou.");

    return EXIT_SUCCESS;
}
