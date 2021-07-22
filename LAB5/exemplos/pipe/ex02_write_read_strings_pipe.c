// Fonte: http://www.gnu.org/software/libc/manual/html_node/Creating-a-Pipe.html#Creating-a-Pipe
// Usando funções putchar e fprintf para ler e escrever em pipes
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Lê os caracteres do pipe e imprime em stdout. */
void read_from_pipe(int file) {
	FILE *stream;
	int c;
	stream = fdopen(file, "r");
	while ((c = fgetc(stream)) != EOF)
		putchar(c);
	fclose(stream);
}

/* Escreve um texto no pipe. */
void write_to_pipe(int file) {
	FILE *stream;
	stream = fdopen(file, "w");
	fprintf(stream, "hello, world!\n");
	fprintf(stream, "goodbye, world!\n");
	fclose(stream);
}

/* Programa principal */
int main(void) {
	pid_t pid;
	int mypipe[2];

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
		read_from_pipe(mypipe[0]);
		return EXIT_SUCCESS;
	} else {
		/* Processo pai. */
		close(mypipe[0]);
		write_to_pipe(mypipe[1]);
		return EXIT_SUCCESS;
	}
}
