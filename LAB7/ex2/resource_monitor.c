#include "resource_monitor.h"

// Initialize the mutex and condition variables
void initMonitor(){
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condNoel, NULL);
    pthread_cond_init(&elfoWait, NULL);
    pthread_cond_init(&renaWait, NULL);

    elfoCount = 0;
    renaCount = 0;
    for(int i=0; i<8; i++) bufferRena[i] = -1;
    for(int i=0; i<3; i++) bufferElfo[i] = -1;
}


//add rena ou elfo no buffer
int put(TIPO tipo, int thread_id){

    //evitar duplicacao, nao executar a funcao se a thread que a chamou ja tiver no buffer
    int flag = 0;
    for(int i=0; i<renaCount; i++)
        if(thread_id == bufferRena[i])
            flag = 1;
    for(int i=0; i<elfoCount; i++)
        if(thread_id == bufferElfo[i])
            flag = 1;
    if(flag == 1) return 1;

    //programa a partir dessa linha
    pthread_mutex_lock(&the_mutex);
    
    //rena
    if(tipo == RENA){
        //espera o buffer se esvaziar
        while(renaCount >= 9)
            pthread_cond_wait(&renaWait, &the_mutex);

        //se houver vaga no estabulo, colocar a rena que chegou
        if(renaCount < 8){
            bufferRena[renaCount] = thread_id;

        //se nao houver espaco, entao essa e a ultima rena, portanto chamar o noel
        }else pthread_cond_signal(&condNoel);
        renaCount++;

    //elfo
    }else if (tipo == ELFO){ 
        //espera o buffer se esvaziar
        while(elfoCount >= 3)
            pthread_cond_wait(&elfoWait, &the_mutex);

        //coloca o elfo na fila
        bufferElfo[elfoCount] = thread_id;
        elfoCount++;

        //se a fila estiver cheia, chamar o noel
        if(elfoCount >= 3) pthread_cond_signal(&condNoel);
    }
    
    pthread_mutex_unlock(&the_mutex); /* release the buffer */
    return 0;
}


//funcao exclusiva do papai noel
TIPO take(){
    pthread_mutex_lock(&the_mutex); /* protect buffer */
    TIPO tipo;

    //papai noel esperando por qualquer chamado
        while(renaCount < 9 && elfoCount < 3)             /* If there is something  in the buffer then wait */
            pthread_cond_wait(&condNoel, &the_mutex);

    //rena
    if(renaCount >= 9){
        //reseta o buffer
        for(int i=0; i<8; i++) bufferRena[i] = -1;
        renaCount = 0;

        //manda sinal para as renas para que possam se juntar no buffer de novo
        pthread_cond_signal(&renaWait);
        tipo = 0;
    }

    //elfo
    //a utilizacao do else if deve priorizar as Renas, pois so sera executado se o if falhar
    else if(elfoCount >= 3){
        //reseta o buffer
        for(int i=0; i<3; i++) bufferElfo[i] = -1;
        elfoCount = 0;

        //manda sinal para os elfos para que possam se juntar no buffer de novo
        pthread_cond_signal(&elfoWait);
        tipo = 1;
    }

    pthread_mutex_unlock(&the_mutex); /* release the buffer */

    //retorna se papai noel limpou o buffer dos elfos ou das renas. Tipo 0 = rena, tipo 1 = elfo
    return tipo;
}


// Cleanup -- would happen automatically at end of program
void destroyMonitor(){
    pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
    pthread_cond_destroy(&condNoel);      /* Free up consumer condition variable */
    pthread_cond_destroy(&elfoWait);      /* Free up consumer condition variable */
    pthread_cond_destroy(&renaWait); 
}
