/*------------------------------------------------------------------------
 * Programa:
 *    chat_servidor - Servidor Chat
 *
 * Funcionalidades:
 *    - alocar um socket para receber conexões
 *    - limitar o numero de conexões
 *    - mapear o processo a uma porta
 *    - aguardar as conexões
 *    - iniciar um socket para a comunicação com o cliente
 *    - enviar/receber as jogadas
 *
 *
 * Sintaxe:    chat_servidor [porta]
 *
 *    - porta - número da porta que o servidor está usando
 *
 * Notas:
 *    - se porta não for fornecida é usado um valor de porta padrão
 *------------------------------------------------------------------------
 */

#include <sys/types.h>              /* tipos básicos do GNU C */
#include <sys/socket.h>             /* funções, tipos e constantes para sockets (UNIX) */
#include <netinet/in.h>             /* funções, tipos e constantes para sockets (Internet) */
#include <netdb.h>                  /* dados sobre a rede (ex.: hosts, protocolos, ...) */

#include <stdio.h>                  /* entrada/saída (I/O) */
#include <string.h>                 /* manipulação de strings */
#include <stdlib.h>                 /* exit() */
#include <unistd.h>                 /* close() */

#define IP_SERVER     "127.0.0.1"   /* especifica o IP do servidor */
#define PORTA_PADRAO  5000          /* especifica a porta padrao de conexao */
#define MAX_CLIENT    10            /* maximo de conexoes */
#define MAX_BUFFER    100           /* tamanho máximo do buffer */

int main(int argc, char *argv[]) {

	struct hostent *ptrh; /* ponteiro para a tabela de hosts */
	struct sockaddr_in addr_server, /* estrutura para armazenar o IP e a porta do servidor */
	addr_client; /* estrutura para armazenar o endereco do cliente */
	int serverSocket, /* socket para ouvir conexões */
	playerSocket; /* socket para comunicação com o jogador conectado */
	int port; /* porta utilizada na comunicação */
	char buffer[MAX_BUFFER]; /* armazena os dados recebidos */

	/* verifica os parâmetros passados por linha de comando */
	if (argc > 1) port = atoi(argv[1]); /* converte para inteiro */
	else port = PORTA_PADRAO; /* usa a porta padrão */

	/* testa a validade da porta */
	if ((port <= 1024) || (port >= 65536)) {
		fprintf(stderr,"número de porta inválido %s\n",argv[2]);
		exit(1);
	} //if

	/* obtem o endereco do servidor (isto é, o endereço local) */
	ptrh = gethostbyname(IP_SERVER);

	/* preenche com zeros a estrutura sockaddr_in */
	memset((char *)&addr_server,0,sizeof(addr_server));

	/* preenche a estrutura endereco */
	addr_server.sin_family = AF_INET; /* define familia como Internet  */
	addr_server.sin_port = htons(port); /* define a porta */
	addr_server.sin_addr.s_addr = INADDR_ANY; /* seta o endereco IP local */
	//memcpy(&addr_server.sin_addr, ptrh->h_addr, ptrh->h_length);   /* especifica o IP */

	/* cria um socket servidor TCP */
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	/* testa para verificar sucesso */
	if (serverSocket < 0) {
		fprintf(stderr, "falha na criação do socket\n");
		exit(1);
	} //if

	/* mapeia um endereco local para o socket */
	if (bind(serverSocket, (struct sockaddr *)&addr_server, sizeof(addr_server)) < 0) {
		fprintf(stderr,"erro no mapeamento da porta (bind)\n");
		exit(1);
	} //if

	/* Especifica o numero de conexoes suportadas */
	if (listen(serverSocket, MAX_CLIENT) < 0) {
		fprintf(stderr,"erro em limitar o número de conexões (listen)\n");
		exit(1);
	} //if

	while (1) {
		/* Aguarda por conexoes */
		int szAddressClient = sizeof(addr_client);
		printf ("Servidor iniciado e aguardando conexões ...\n");
		if ( (playerSocket=accept(serverSocket, (struct sockaddr *)&addr_client,(socklen_t *) &szAddressClient)) < 0) {
			fprintf(stderr, "erro ao aceitar conexão (accept)\n");
			exit(1);
		} // if

		/* protocolo de comunicação (enviar e receber dados) */
		recv (playerSocket, buffer, sizeof (buffer), 0); /* recebe dados */
		strcpy (buffer,"Conexao aceita.\n");
		send (playerSocket, buffer, sizeof (buffer), 0); /* envia dados */

		buffer[MAX_BUFFER-1] = 0x0;
		printf ("%s\n", buffer);
		printf ("Cliente conectado \n\n");

		/* protocolo para o chat */
		printf ("------------------------------------------------\n");
		printf ("--               Chat Servidor                --\n");
		printf ("------------------------------------------------\n\n");

		while (1) {
			recv (playerSocket, buffer, sizeof (buffer), 0); /* recebe dados */
			printf ("Mensagem (Cliente): %s\n", buffer);

			if (strcmp(buffer, "sair") == 0) break;

			printf("Mensagem (Servidor): ");
			gets(buffer);
			send (playerSocket, buffer, sizeof (buffer), 0); /* envia dados */
		} //while

		/* fecha a conexão */
		close(playerSocket);

	} //while

	close(serverSocket);

	/* finaliza o programa */
	exit(0);
} // main
