#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Programa principal */
int main(void) {
	pid_t pid;
	int mypipe[2];
    char buffer[40];

	/* Criar o pipe. */
	if (pipe(mypipe)) {
		fprintf(stderr, "Falha ao criar o Pipe.\n");
		return EXIT_FAILURE;
	}

	/* Criar o processo filho. */
	pid = fork();
	if (pid < (pid_t) 0) {
        /* pid negativo, falha no fork. */
        fprintf(stderr, "Falha ao executar o Fork.\n");
        return EXIT_FAILURE;
	} else if (pid == (pid_t) 0) {
        /* No processo filho. */
        close(mypipe[1]);
        read(mypipe[0], buffer, sizeof(buffer));
        printf("FILHO: ...%s\n", buffer);
        fflush(stdout);
        return EXIT_SUCCESS;
	} else {
		/* Processo pai. */
		close(mypipe[0]);
        printf("PAI: Digite algo para enviar: ");
        scanf("%40[^\n]", buffer);
		write(mypipe[1], buffer, sizeof(buffer));
        
        wait(NULL);
		return EXIT_SUCCESS;
	}
}
