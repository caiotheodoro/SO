// Programa com N threads ping e N threads pong,
// Tem como função intercalar as threads ping e pong de uma maneira que seja
//imprimido ping e pong sequencialmente.

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
// Datas: github.com/caiotheodoro/SO/commits/master

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //warning sleep
#include <pthread.h>
#include <signal.h> // pthread_kill
#include <time.h> // time(NULL)


typedef struct Sync{
    int* mensagens;
    char* pp;
    int binario;
    pthread_mutex_t *mutex;
} Sync;


//Sync* criaSync(int *mensagens, char* pp, int binario, pthread_mutex_t mutex);
void* pingpong(void* val);


int main(int argc, char *argv[]){

int pingN = atoi(argv[1]);
int pongN = atoi(argv[2]);

pthread_t threadPing[pingN];
pthread_t threadPong[pongN];

pthread_mutex_t mutex;
int contador = 0;
Sync ping, pong;
//ping = criaSync(contador, "ping", 0, mutex);
//pong = criaSync(contador, "pong", 1, mutex);

ping.mensagens = &contador; //inicializa com 
ping.pp = "ping";
ping.binario = 0;
ping.mutex = &mutex;

pong.mensagens = &contador;
pong.pp = "pong";
pong.binario = 1;
pong.mutex = &mutex;



//cria as threads
for(int i=0; i<pingN; i++){//cria as thread de tipo PING
    pthread_create(&threadPing[i], NULL, &pingpong, (void*) &ping);
   // printf("id thread ping N %d = %d",i,threadPing);
}
for(int i=0; i<pongN; i++){ //cria as thread de tipo PONG
    pthread_create(&threadPong[i], NULL, &pingpong, (void*) &pong);
  //  printf("id thread pong N %d = %d",i,threadPing);
}


int total = pingN + pongN;
int countPing = 0, countPong = 0;

for(int i=0; i<total; i++){//modelo de inserção a=1 b=1  a=2 b=2  a=3 b=3 a=fim b=4 5 6 7
    if((i % 2 == 0) && (countPing != pingN) || countPong == pongN){ //verifica se é par E se acabou suas threads, ou a do outro .
    pthread_join(threadPing[countPing], NULL);
    countPing++;//soma
    }
    else {
    pthread_join(threadPong[countPong], NULL);
    countPong++; //soma
    }

}

    pthread_mutex_destroy(&mutex); //finaliza o mutex

}

void* pingpong(void* val){

    Sync *struc;
    int contador;

    struc = val;
    while(1){
        pthread_mutex_lock(struc->mutex); //bloqueia o dado para a thread
        if(*struc->mensagens % 2 == struc->binario) { //verifica se é ping ou pong baseado nas mensagens enviadas 
            printf("%s\t", struc->pp); //print ping;pong
            printf("thread id = %d\n", pthread_self()); //print ID da thread
            contador = *struc->mensagens; //atribui ao contador 
            sched_yield();//thread_yield(), libera a CPU;
            contador +=  1;//conta +1 ping;pong
            *struc->mensagens = contador;//guarda na struct
            sleep(1); //aguarda 1s
        }
        pthread_mutex_unlock(struc->mutex);
        sched_yield(); //thread_yield() libera a CPU;
    }
}

/*Sync* criaSync(int *mensagens, char* pp, int binario, pthread_mutex_t mutex){ 
    Sync* novo = (Sync*)malloc(sizeof(Sync));
    novo->mensagens = mensagens;
    novo->pp = pp;
    novo->binario = binario;
    novo->mutex = mutex; //&
    return novo;

}
*/