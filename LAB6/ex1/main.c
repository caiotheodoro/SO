// Programa com N threads que calcula a mediana das linhas de uma matriz
// e a média das suas colunas. além de calcular seu tempo de execução.
// Ademais, o programa salva os resultados obtidos em um arquivo.

// AUTORES: Caio Theodoro, Caio Miglioli, Alexandre Scrocaro
// Datas: github.com/caiotheodoro/SO/commits/master

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>
//#include <signal.h> // pthread_kill
#include <time.h> // time(NULL)

//variavel global
pthread_mutex_t mutex;

//definicoes
typedef struct datachunk {
    int** matrix;
    int mLinhas;
    int mColunas;
    int* resultado;
    int* controle;
} datachunk;

//funcoes utilitarias
void* matrixSum(void* chunk);
datachunk* dc_createChunk(int linhas, int colunas, int* controle, int* resultado, int** matrix);
int** createMatrix(int linhas, int colunas, int modulo);
void destroyMatrix(int** matrix, int colunas);

/////////////////////////////////////////
//////////////// PROGRAMA ///////////////
/////////////////////////////////////////

int main(int argc, char *argv[]){
    if(argc < 4) return printf("./$ <N_Threads> <m_Linhas> <m_Colunas>\n");

    //variaveis
    int n_of_threads = atoi(argv[1]);
    int mLinhas = atoi(argv[2]);
    int mColunas = atoi(argv[3]);
    pthread_t thread[n_of_threads];
    int controle = 0;

    //cria os vetores e o DC (variaveis dinamicas)
    int** matrix = createMatrix(mLinhas, mColunas, 100);
    int* resultado = (int*)calloc(mLinhas, sizeof(int));
    datachunk* chunk = dc_createChunk(mLinhas, mColunas, &controle, resultado, matrix);
    
    printf("Programa iniciado com uma matriz %dx%d\n", mLinhas, mColunas);

    //inicia o mutex
    pthread_mutex_init(&mutex, NULL);

    //cria as threads
    for(int i=0; i<n_of_threads; i++){
        pthread_create(&thread[i], NULL, matrixSum, (void*) chunk);
    }

    //espera elas retornarem
    for(int i=0; i<n_of_threads; i++){
        pthread_join(thread[i], NULL);
    }

    //finaliza o mutex
    pthread_mutex_destroy(&mutex);

    //mostrar resultados
    printf("\nThreads finalizadas.\n\nVetor de Resultados: [");
    for(int i=0; i<mLinhas-1; i++){
        printf("%d, ", resultado[i]);
    }
    printf("%d]\n", resultado[mLinhas-1]);

    //destroi as memorias dinamicas
    destroyMatrix(matrix, mColunas);
    free(chunk);
    free(resultado);

    printf("Main finalizado\n");
    //pthread_exit(0);
    return 0;
}

// THREAD
void* matrixSum(void* chunk){
    //conversao de type
    datachunk* dc = (datachunk*)chunk;

    //informacoes locais
    int workqtde = 0;
    int workdone[dc->mLinhas];
    for(int i=0; i<dc->mLinhas; i++)
        workdone[i] = 0;

    //pedindo constantemente uma nova linha para ler
    while(*(dc->controle) < dc->mLinhas){

        //inicia o bloqueio
        pthread_mutex_lock(&mutex);

        //soma
        int soma = 0;
        for(int i=0; i<dc->mColunas; i++){
            soma += dc->matrix[ *(dc->controle) ][i];
            //printf("%dx%d - item %d, soma %d\n", *(dc->controle),i,dc->matrix[ *(dc->controle) ][i],soma);
        }
        
        //guarda na vetor de resultados
        dc->resultado[*(dc->controle)] = soma;
        //printf("resul %d, cntrl %d\n", dc->resultado[*(dc->controle)], *(dc->controle));

        //Acrescentar variaveis de controle local
        workdone[workqtde] = *(dc->controle);
        workqtde++;

        //Acrescentar variavel de controle
        *(dc->controle) += 1;

        //termina o bloqueio
        pthread_mutex_unlock(&mutex);

        //cria atraso no processo para que possa dividir as linhas para cada thread.
        for(int i = 0; i<1000000; i++);
    }

    //mostra em quais linhas essa thread operou
    printf("\nThread - Linhas lidas: ");
    for(int i=0; i<workqtde; i++) printf("%d, ", workdone[i]);
}

/////////////////////////////////////////
////////// FUNCOES UTILITARIAS //////////
/////////////////////////////////////////

datachunk* dc_createChunk(int linhas, int colunas, int* controle, int* resultado, int** matrix){ 
    datachunk* dc = (datachunk*)malloc(sizeof(datachunk));
    dc->mLinhas = linhas;
    dc->mColunas = colunas;
    dc->controle = controle;
    dc->resultado = resultado;
    dc->matrix = matrix;
    return dc;
}

int** createMatrix(int linhas, int colunas, int modulo){
    //alocando
    int** matrix = (int**)malloc(linhas * sizeof(int*));
    for(int i=0; i<linhas; i++)
        matrix[i] = (int*)malloc(colunas * sizeof(int));

    //inserindo valores aleatorios
    srand(time(NULL));
    for(int i=0; i<linhas; i++)
        for(int j=0; j<colunas; j++)
        matrix[i][j] = rand()%100;
    return matrix;
}

void destroyMatrix(int** matrix, int colunas){
    for(int i=0; i<colunas; i++)
        free(matrix[i]);
    free(matrix);
}