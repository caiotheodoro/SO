#include <stdio.h>
#include <stdlib.h>

typedef struct Data {
    int pos_i, pos_f;
    int **matriz;
    int seq;
    int lines;
    int columns;
}Data;


Data* criaData(int ini, int fim, int seq){
  Data* novo = (Data*)malloc(sizeof(Data));
  novo->pos_i = ini;
  novo->pos_f = fim;
  novo->matriz = NULL;
  novo->seq = seq;
  return novo;
}

void sizeMatriz(Data* data)
{
    FILE *arquivo = fopen("matriz.in", "r");
    int lines = 0;
    char palavra[255];
    char *linha;

    linha = fgets(palavra, 255, arquivo);
    data->lines = (int)linha[0] - '0';
    data->columns = (int)linha[2] - '0';

    fclose(arquivo);
    return;
}

void insereMatriz(Data *data)
{

    FILE *arquivo;
    char palavra[100];
    char *linha;
    unsigned int i = 0;

    arquivo = fopen("matriz.in", "r");
    if (arquivo == NULL)
    {
        printf("arquivo vazio");
    }
    while (!feof(arquivo))
    {
        linha = fgets(palavra, 100, arquivo);
        if (linha && i != 0)
        {
          for(int j=0;j<data->columns;j++){
          
            data->matriz[i][j] = (int) linha[j*2] - '0';
            
          }
         
            i++;
        }
    }


    fclose(arquivo);
}

int main(void) {

  Data* novo = criaData(0, 10, 0);
  sizeMatriz(novo);
  printf("%d %d", novo->lines, novo->columns);
  insereMatriz(novo);
  
  

  return 0;
}