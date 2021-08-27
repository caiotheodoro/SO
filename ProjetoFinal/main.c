#include <stdio.h>
#include "fatacc.h"

#include <string.h>

#define BLOCKSIZE 4096
#define BLOCKQTDE 12

// typedef struct{
//     char x[20];
// } Entry;

int main(int argc, char* argv[]){
    facc_format(BLOCKSIZE, BLOCKQTDE);
    // Entry entries[2];
    // printf("%d\n", sizeof(entries));
    return 1;
}