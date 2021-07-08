/** 
 * Exemplo básico de driação de threads com pthred.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *ptr );

int main() {
     /* Declara duas threads */
     pthread_t thread1, thread2;
     char *message1 = "Olá eu sou a Thread 1";
     char *message2 = "Olá eu sou a Thread 2";
     int  iret1, iret2;

     /* Cria duas threads independentes, cada uma irá executar a função */
     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);

     /* Aguarda até que todas as threads completem antes de continuar. */
     /* Pode acontecer de executar algo que termine o processo/thread principal antes das threads terminarem */
     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL); 

     printf("Thread 1 retornou: %d\n", iret1);
     printf("Thread 2 retornou: %d\n", iret2);
     
     exit(0);
}

void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     printf("%s \n", message);
     
     pthread_exit(NULL);
}
