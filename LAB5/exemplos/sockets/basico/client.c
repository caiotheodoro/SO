/* client.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORTA_SERVER 30000

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Sintaxe:\nclient IPSERVIDOR");
        return 1;
    }       

	int client_socket, numbytes;
	int bufsize = 1024;
	char *buffer = malloc(bufsize);
	struct sockaddr_in server_address;

	printf("Criando socket...\n");
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0) {
		printf("Socket criado.\n");
	} else {
		perror("Erro criando o socket.");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORTA_SERVER);
	inet_pton(AF_INET, argv[1], &server_address.sin_addr);

	printf("Tentando se conectar ao servidor: %s\n", argv[1]);

	if (connect
	    (client_socket, (struct sockaddr *)&server_address,
	     sizeof(server_address)) == 0) {
		printf("Conexão estabelecida com o servidor %s...\n",
		       inet_ntoa(server_address.sin_addr));
	} else {
		perror("Erro conectando ao servidor.");
		exit(1);
	}

	if ((numbytes = recv(client_socket, buffer, bufsize, 0)) == -1) {
		perror("Erro no receive");
		exit(1);
	}

	buffer[numbytes] = '\0';
	printf("Mensagem de Boas Vindas: %s\n", buffer);

	if (strcmp(buffer, "Opa!")) {
		printf("Enviando mensagem...\n");
		fgets(buffer, bufsize - 1, stdin);
		send(client_socket, buffer, bufsize, 0);
		printf("Mensagem Enviada: %s\n", buffer);
	}

	close(client_socket);

	return 0;
}
