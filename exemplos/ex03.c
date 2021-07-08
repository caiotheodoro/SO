/**
 * Código de exemplo para paralelização de dados com pthreads.
 * Problema: Contabilizar o número de consoantes e vogais em um arquivo texto.
 * Solução paralela: Atribuir a contabilização para threads. Cada thread processa
 * uma parte do texto. Ao final, junta-se as respostas.
 */

#include<stdio.h>
#include<pthread.h>
#include<ctype.h>

/* especifica o número de threads e partes */
#define N 3

/* estrutura para passar os parâmetros para as threads */
struct data_chunk {
    int num_seq;
    int pos_inicio, pos_final;
    char *texto;
};

/* variavel global para armazenar as respostas para N threads */
/* coluna 0 = total de vogais e coluna 1 = total consoantes */
int matriz_respostas[N][2];   


/* Thread para contabilizar o número de vogais e consoantes
   de um trecho de texto informado como param */
void *thread_cont(void *param) {
    struct data_chunk *dados = param;

    int cont_vogal, cont_consoante;
    cont_vogal = cont_consoante = 0;

    printf("Thread id: %ld, Posicao inicio: %d, Posicao final: %d\n",
        pthread_self(), dados->pos_inicio, dados->pos_final);

    for (int j=dados->pos_inicio; j<=dados->pos_final; j++) {
        char letra = tolower(dados->texto[j]);

        if (letra == 'a' || letra == 'e' || letra == 'i' || letra == 'o' || letra == 'u') cont_vogal++;
        else if (letra >= 'a' && letra <= 'z') cont_consoante++;
    }

    printf("vogais: %d, consoantes: %d\n", cont_vogal, cont_consoante);
    matriz_respostas[dados->num_seq][0] = cont_vogal;
    matriz_respostas[dados->num_seq][1] = cont_consoante;
    
    return NULL;
}

int main() {
    printf("Exemplo - Paralelismo de dados com threads\n\n");

    /* lê o arquivo texto */
    FILE *file = fopen("texto.txt", "r");
    fseek (file, 0, SEEK_END);
    int tamanho = ftell (file);
    printf("Tamanho do arquivo (bytes): %d\n", tamanho);

    char texto[tamanho + 1];  // conteudo do arquivo    

    char c;
    int i = 0;
    rewind(file);
    while ((c = getc(file)) != EOF)
        texto[i++] = c;
    texto[i] = 0;
    fclose(file);

    /* prepara os dados para as threads - divide em partes */
    struct data_chunk dados[N];

    int tam_parte = tamanho/N;
    int ini_parte = 0;
    int fim_parte = tam_parte;
    for (int p=0; p < N; p++) {
        dados[p].num_seq = p;
        dados[p].pos_inicio = ini_parte;
        dados[p].pos_final = fim_parte;
        dados[p].texto = texto;

        ini_parte = fim_parte + 1;
        fim_parte += tam_parte;
    }
    dados[N-1].pos_final = tamanho;

    /* declara, cria e invoca as threads */
    pthread_t t[N];

    for (int p=0; p < N; p++) 
        pthread_create(&t[p], NULL, thread_cont, &dados[p]);

    /* aguarda a finalização das threads */
    for (int p=0; p < N; p++) 
        pthread_join (t[p], NULL);

    /* junta os resultados das threads */
    int total_consoantes, total_vogais;
    total_consoantes = total_vogais = 0;
    for (int i=0; i<N; i++) {
        total_vogais += matriz_respostas[i][0];
        total_consoantes += matriz_respostas[i][1];
    }
    printf("Total de vogais: %d\nTotal de consoantes: %d\n", 
        total_vogais, total_consoantes);

    return 0;
}
