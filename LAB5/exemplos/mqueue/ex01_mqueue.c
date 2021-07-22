/**
 * Envia mensagens de uma thread para outra.
 * Original obtido em https://gist.github.com/gustavorv86/9e98621b44222114524399a3b4302ddb
**/

#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_FILE "/tmp/myqueue.log"

#define QUEUE_NAME  "/myqueue"
#define MAX_SIZE    256         /* tamanho do buffer */

void *queue_server(void * args) {
    mqd_t mq;                   /* descritor da fila de mensagens */  
    ssize_t bytes_read;         /* contagem de bytes ou indicação de erro - sys/types.h*/
    struct mq_attr attr;        /* atributos da fila de mensagens */
    char buffer[MAX_SIZE + 1];  /* buffer para armazenar os dados */
    
    
    FILE *logfile;
    /* abrir arquivo de log */
    logfile = fopen(LOG_FILE, "a+");
    
    /* inicializar os atributos */
    attr.mq_flags = 0;          /* flags */
    attr.mq_maxmsg = 10;        /* número máximo de mensagens */
    attr.mq_msgsize = MAX_SIZE; /* tamanho máximo da mensagem */
    attr.mq_curmsgs = 0;        /* número de mensagens na fila */

    /* cria a fila de mensagens */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY , 0644, &attr);

    do {
        memset(buffer, 0x00, sizeof(buffer));   // zera o conteúdo do buffer 
        /* aguarda o recebimento de mensagens */
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);    /* passa o descritor, msg, tamanho máximo, prioridade */

        if(bytes_read >= 0) {
            fprintf(logfile, "SERVER: Received message: %s", buffer);
            fflush(logfile);
        } else {
            fprintf(logfile,"SERVER: None \n");
        }
        
    } while (strcmp(buffer, "exit\n") != 0);

    /* desacoplar o descritor e remover a fila do sistema */
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    
    /* fechar arquivo */
    fclose(logfile);

    return NULL;
}


void *queue_client(void * args) {
    
    mqd_t mq;
    char buffer[MAX_SIZE];

    /* acessa a fila para escrita */
    mq = mq_open(QUEUE_NAME, O_WRONLY);

    int count = 0;
    do {
        printf("Message: ");
        fflush(stdin);
        fgets(buffer, sizeof(buffer), stdin);
        
        printf("CLIENT: Send message... \n");
        mq_send(mq, buffer, MAX_SIZE, 0);    /* passar o descritor, msg, tamanho máximo, prioridade */
        
        fflush(stdout);
    } while (strcmp(buffer, "exit\n")!=0);

    /* desacopla o descritor */
    mq_close(mq);

    return NULL;
}


int main(int argc, char** argv) {

    pthread_t client, server;
    
    printf("Start...\n");
    
    pthread_create(&server, NULL, &queue_server, NULL);
    pthread_create(&client, NULL, &queue_client, NULL);
    
    pthread_join(server, NULL);
    pthread_join(client, NULL);
    
    printf("Done...\n");
    
    return (EXIT_SUCCESS);
}

 
