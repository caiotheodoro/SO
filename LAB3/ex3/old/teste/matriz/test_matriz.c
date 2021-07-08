/**
 * Program: test_matriz.c
 * Descrição: testar as funções da biblioteca matriz.h
 * 
 * Autor: Rodrigo Campiolo
 * Data: 04/09/2019
 */

#include <stdio.h> 
#include <stdlib.h>     //atoi(), srand()
#include <time.h>       //time() 
#include "matriz.h"

#define M 10
#define N 10

 
 /********** main - fluxo principal **********/
int main(int argc, char *argv[]) 
{ 
   int r = M, c = N, i, j, count; 
    
   /* processa as dimensões da matriz se informadas por parâmetro */
   if (argc == 3) {        
      r = atoi(argv[1]);
      c = atoi(argv[2]);
   }

   /* lê a matriz a partir de um arquivo de entrada */   
   int **matrix = read_matrix_from_file("data_matriz_100_200.in", &r, &c);

   /* gera uma matrix rxc e popula com valores pseudoaleatorios */
   //int **matrix = create_matrix(r, c);
   //srand(time(NULL));   // inicializa o gerador com uma semente.
   //generate_elements(matrix, r, c, 99);

   printf("%dx%d\n", r, c);
   print_matrix(matrix, r, c);

   return 0; 
} 
