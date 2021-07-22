/*
 * Fonte: http://www.cs.cf.ac.uk/Dave/C/node27.html
 */
/*
 * shm-client - client program to demonstrate shared memory.
 * 
 * int shmget(key_t key, size_t size, int shmflg): devolve identificador do segmento de memória compartilhada
 * void *shmat(int shmid, const void *shmaddr, int shmflg): acopla o segmento de memória compartilhada  ao espaço do processo.
 * int shmdt(const void *shmaddr);: desacopla o segmento de memória compartilhada.
 *  shmctl(shmid, IPC_RMID, NULL): remove um segmento de memória compartilhada.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ     27

int main() {
	int shmid;
	key_t key;
	char *shm, *s;

	/* É preciso pegar o segmento nomeado como "5678", criado pelo servidor. */
	key = 5678;

	/* Localizando o segmento (locate). */
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
		perror("Erro ao tentar acessar o segmento de shm (shmget).");
		exit(1);
	}

	/* Acoplamento do segmento ao espaço de endereços de dados. */
	if ((shm = shmat(shmid, NULL, 0)) == (char*)-1) {
		perror("Erro ao acoplar o segmento ao espaço de dados do programa (shmat).");
		exit(1);
	}

	/* Lendo o que o servidor colocou na região de memória. */
	for (s = shm; *s != '\0'; s++)
		putchar(*s);
	putchar('\n');

	/* Modificando o primeiro caracter do segmento para '*', indicando que os dados já foram lidos. */
	*shm = '*';

	/* Elimina a região de memória compartilhada. */
	if (shmdt(shm) == -1) {
		perror("Erro ao desacoplar a região de memória compartilhada (shmdt).");
		exit(1);
	}

	return EXIT_SUCCESS;
}
