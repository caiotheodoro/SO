#include <stdlib.h>
#include "datachunk.h"

// define os dados que serão utilizados nos calculos
dc_t* dc_criaData(int** matrix, int r, int c, int ini, int fim, float* array){ 
  dc_t* novo = (dc_t*)malloc(sizeof(dc_t));
  novo->matrix = matrix;
  novo->r = r;
  novo->c = c;
  novo->ini = ini;
  novo->fim = fim;
  novo->array = array;
  return novo;
}