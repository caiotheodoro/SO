// https://docs.google.com/document/d/1kj-KGVFwcZHi_6a92hfFF0w3bnovdMbHGEYD8j9pZwI/edit?usp=sharing

// fatacc.dsc
// ┌──────────────┬─────────┬─────────┬─────────┬─────────┬─────────┬──────────┐
// │   block#0    │ block#1 | block#2 | block#3 | block#4 | block#i | block#n  │
// │  Superbloco  │   Fat   │ DirRoot |  data#0 |  data#1 |   ...   | data#n-3 │  
// └──────────────┴─────────┴─────────┴─────────┴─────────┴─────────┴──────────┘
//  <─ blockSize ─>
//
// blockQtde= n
// fatPos   = block#1
// rootPos  = block#2
// blockPos = block#3 (data#0)

#include <limits.h>

#define DISCO "disco.txt"

// Superbloco
typedef struct superblock{
    char tipo[8];   //8b

    //fat
    int fatPos;     //4b

    //diretorio raiz
    int rootPos;    //4b

    //blocos
    int blockPos;   //4b
    int blockQtde;  //4b
    int blockSize;  //4b
} Superblock;       //28b


// Fat
typedef int Fat;            //bytes = 4 * Superbloco.blockQtde
#define FAT_L (INT_MAX)     // Ultimo bloco do arquivo (Last)
#define FAT_F (INT_MAX-1)   // Bloco livre (Free)
#define FAT_R (INT_MAX-2)   // Bloco reservado (Reserved)
#define FAT_B (INT_MAX-3)   // Bloco defeituoso (Bad)

// Blocos: Arquivos
typedef char file_t;
typedef struct filechunk{
    file_t* file;
    int bytes;
} file_chunk;

// Blocos: Diretorios
typedef struct entry{
    char name[8];       //8b
    char type[4];       //4b extensao (DIR se for diretorio)
    int bytes;          //4b tamanho do arquivo
    int firstBlock;     //4b primeiro bloco onde se encontra o arquivo
    char reserved[12];  //12b para completar 32b e ser potencia de 2
} Entry;                //32b

typedef struct dirmeta{
    char name[8];       //8b
    int entryQtde;      //4b
    int bytes;          //4b tamanho do arquivo
    int firstBlock;     //4b primeiro bloco onde se encontra o arquivo
    char reserved[12];  //12b
} DirMeta;              //32b deve ser igual ou maior que Entry

/* ============================================================= */

void facc_format(int blockSize, int blockQtde);



// struct dirmeta
// struct entry . (bloco ini = procurarBlocoLivre)
// struct entry .. (bloco ini = blocoAtual)