/**
 * Código de exemplo para paralelização de funções com pthreads.
 * Problema: Contabilizar o número de consoantes, vogais e palavras em um arquivo texto.
 * Solução paralela: Atribuir a contabilização de consoantes e vogais para uma thread e
 * a contabilização de palavras para outra thread concorrente.
 */

#include<stdio.h>
#include<pthread.h>
#include<ctype.h>

/* estrutura para passar os parâmetros para as threads */
struct data_chunk {
    int num_seq;
    int pos_inicio, pos_final;
    char *texto;
};


/* Thread para contabilizar o número de vogais e consoantes
   de um trecho de texto informado como param */
void *thread_conta_vogais_consoantes(void *param) {
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
    
    return NULL;
}


/* Thread para contabilizar o número de palavras
   de um trecho de texto informado como param */
void *thread_conta_palavras(void *param) {
    struct data_chunk *dados = param;

    printf("Thread id: %ld, Posicao inicio: %d, Posicao final: %d\n",
        pthread_self(), dados->pos_inicio, dados->pos_final);

    int cont_palavras = 0, num_letras = 0;

    /* algoritmo extremamente simplificado para contagem de palavras em inglês */
    for (int j=dados->pos_inicio; j<=dados->pos_final; j++) {
        char letra = tolower(dados->texto[j]);
        
        if (letra >='a' && letra <='z') num_letras++;
        else {
            if (num_letras > 0) cont_palavras++;
            num_letras = 0;
        }
    } 

    printf("número de palavras: %d\n", cont_palavras);
    
    return NULL;
}


int main() {
    printf("Exemplo - Paralelismo de funções com threads\n\n");

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
    struct data_chunk dados;

    dados.num_seq = 0;
    dados.pos_inicio = 0;
    dados.pos_final = tamanho;
    dados.texto = texto;

    /* declara, cria e invoca as threads */
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_conta_vogais_consoantes, &dados);
    pthread_create(&t2, NULL, thread_conta_palavras, &dados);

    /* ao terminar as threads finaliza */
    pthread_exit(NULL);

    return 0;
}
