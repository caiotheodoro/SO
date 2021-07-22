/*------------------------------------------------------------------------
 * Programa:
 *    chat_cliente - Cliente de um chat
 *
 * Funcionalidades:
 *    - alocar um socket
 *    - conectar ao servidor
 *    - enviar/receber as jogadas
 *
 *
 * Sintaxe:    chat_cliente [host] [porta]
 *
 *    - host  - nome ou ip do computador onde o servidor está executando
 *    - porta - número da porta que o servidor está usando
 *
 * Notas:
 *    - se host não for fornecido é usado o endereço de loopback (127.0.0.1)
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
#define MAX_BUFFER    100           /* tamanho maximo do buffer */

int main(int argc, char *argv[]) {

	struct hostent *ptrh; /* ponteiro para a tabela de hosts */
	struct sockaddr_in address; /* estrutura para armazenar o IP e a porta */
	int sd; /* descritor do socket */
	int port; /* porta utilizada na comunicação */
	char *host; /* armazena o endereço do servidor */
	char buffer[MAX_BUFFER]; /* armazena os dados recebidos */

	/* verifica os parâmetros passados por linha de comando */
	if (argc > 2) port = atoi(argv[2]); /* converte para inteiro */
	else port = PORTA_PADRAO; /* usa a porta padrão */

	/* testa a validade da porta */
	if ((port <= 1024) || (port >= 65536)) {
		fprintf(stderr,"número de porta inválido %s\n",argv[2]);
		exit(1);
	} //if

	/* verifica se o endereço do servidor foi fornecido */
	if (argc > 1) host = argv[1];
	else host = IP_SERVER;

	/* testa a validade do endereco */
	ptrh = gethostbyname(host);
	printf("Host name: %s\n",(char *)ptrh);
	if (ptrh == NULL) {
		fprintf(stderr,"host inválido: %s\n", host);
		exit(1);
	} //if

	/* preenche com zeros a estrutura sockaddr_in */
	memset((char *)&address,0,sizeof(address));

	/* preenche a estrutura endereco */
	address.sin_family = AF_INET; /* define familia como Internet  */
	address.sin_port = htons(port); /* define a porta */
	memcpy(&address.sin_addr, ptrh->h_addr, ptrh->h_length); /* especifica o destino (IP) */

	/* cria um socket TCP */
	sd = socket(PF_INET, SOCK_STREAM, 0);

	/* testa para verificar sucesso */
	if (sd < 0) {
		fprintf(stderr, "falha na criação do socket\n");
		exit(1);
	} //if

	/* conecta o socket ao servidor e testa a conexão */
	if (connect(sd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		fprintf(stderr,"falha na conexão\n");
		exit(1);
	} //if

	/* protocolo de comunicação (enviar e receber dados) */
	printf ("Solicitando conexão ...\n");
	strcpy(buffer, "Solicitando conexão ...!!!\n");
	send (sd, buffer, sizeof (buffer), 0); /* envia dados */
	recv (sd, buffer, sizeof (buffer), 0); /* recebe dados */
	printf ("Mensagem (Remota): %s\n", buffer);

	/* protocolo chat */
	printf ("------------------------------------------------\n");
	printf ("--                   Chat                     --\n");
	printf ("------------------------------------------------\n\n");

	while (1) {
		printf("Mensagem (Local): ");
		gets(buffer);
		send (sd, buffer, sizeof (buffer), 0); /* envia dados */

		if (strcmp(buffer, "sair") == 0) break;

		recv (sd, buffer, sizeof (buffer), 0); /* recebe dados */
		printf ("Mensagem (Remota): %s\n", buffer);
	} //while

	/* fecha a conexão */
	close(sd);
    
    return 0;
} // main
