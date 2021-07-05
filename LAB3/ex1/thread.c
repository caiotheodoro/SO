#include "thread.h"
#include <stdlib.h>
#include <stdio.h>

Data* criaData(int ini, int fim,int*v1){
  Data* novo = (Data*) malloc (sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->v1 = v1;

  return novo;
}