#include "random_vector.h"
#include <stdlib.h>
#include <time.h>

rv* rv_gerar(int tamanho, int modulo){
    rv* vtr = malloc(sizeof(rv));
    vtr->array = calloc(tamanho, sizeof(int));
    vtr->size = tamanho;
    srand(time(NULL));

    for(int i=0; i<tamanho; i++)
        vtr->array[i] = rand() % modulo;

    return vtr;
}

void rv_excluir(rv* vtr){
    free(vtr->array);
    free(vtr);
}