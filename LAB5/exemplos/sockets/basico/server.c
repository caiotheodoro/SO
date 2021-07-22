/* server.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PORTA 30000
#define CONEXOES 10

int main() {

	int server_socket, client_socket, addrlen;
	int bufsize = 1024;
	char *buffer = malloc(bufsize);
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	printf("Olá!\n");
	printf("Iniciando servidor...\n");

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0) {
		printf("Socket criado.\n");
	} else {
		perror("Erro na criação do socket.");
		exit(1);
	}

	// Endereço do servidor.
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;	// Coloca IP automaticamente.
	server_address.sin_port = htons(PORTA);
	bzero(&(server_address.sin_zero), 8);

	if (bind
	    (server_socket, (struct sockaddr *)&server_address,
	     sizeof(struct sockaddr)) == 0) {
		printf("Binding Socket.\n");
	} else {
		perror("Erro ao executar o Binding.");
		exit(1);
	}

	printf("Listening...\n");
	if (listen(server_socket, CONEXOES) < 0) {
		perror("Erro no listen.");
		exit(1);
	}

	addrlen = sizeof(struct sockaddr_in);
	printf("Aceitando conexões de clientes...\n");
	if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address,
		    (socklen_t *)&addrlen)) < 0) {
		printf("Erro no accept.\n");
	}

	if (client_socket > 0) {
		printf("Cliente %s está conectado...\n",
		       inet_ntoa(client_address.sin_addr));
	}

	if (strcmp(buffer, "Opa!")) {
		printf("Mensagem Enviada: ");
		fgets(buffer, bufsize - 1, stdin);
		send(server_socket, buffer, bufsize, 0);
	}

	recv(client_socket, buffer, bufsize, 0);
	printf("Mensagem recebida: %s\n", buffer);

	close(client_socket);
	close(server_socket);

	return 0;
}
