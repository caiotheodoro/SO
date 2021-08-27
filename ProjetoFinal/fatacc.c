#include "fatacc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ========================== funcoes internas ========================= */

file_chunk* createDirectory(char* name, int freeBlock, DirMeta fatherMeta);
void writeBlock(Superblock sb, int blockNum, void* buffer, int bufferSize);
file_t* readBlock(Superblock sb, int blockNum, int bufferSize);

/* ===================================================================== */


void facc_format(int blockSize, int blockQtde){
    /* criar disco */
    FILE* dsc = fopen(DISCO, "w");
    char* tmp2 = (char*)calloc(blockSize, sizeof(char));
    for(int i=0; i<blockQtde; i++) fwrite(tmp2, 1, blockSize, dsc);
    free(tmp2);
    fclose(dsc);    


    /* criar superbloco */
    Superblock sb;
    strcpy(sb.tipo, "fatacc");

    sb.fatPos = 1;
    sb.rootPos = 2;
    sb.blockPos = 3;
    
    sb.blockSize = blockSize;
    sb.blockQtde = blockQtde;

    writeBlock(sb, 0, &sb, sizeof(Superblock));


    /* fat */
    Fat* fat = (Fat*)malloc(sizeof(Fat) * sb.blockQtde);

    for(int i=0; i<3; i++){
        fat[i] = FAT_R; //reservados
    }

    for(int i=3; i<blockQtde; i++){
        fat[i] = FAT_F;
    }

    writeBlock(sb, sb.fatPos, fat, sizeof(Fat)*sb.blockQtde);
    free(fat);

    
    /* dirRoot */
    //cria o que seria o meta do diretorio pai para inserir na entry ".."
    DirMeta fatherMeta;
    fatherMeta.firstBlock = sb.rootPos;
    
    // 1 - encontrar posicao
    // -> por ser root, usara a posicao superbloco.rootPos

    // 2 - criar diretorio filho e guardar no disco
    file_chunk* file = createDirectory("root", sb.rootPos, fatherMeta);

    // 3 - criar entry no diretorio pai e guardar no disco
    // -> nao ha diretorio pai pois este eh o root
           
    // printf("file size %d", file->bytes);
    writeBlock(sb, sb.rootPos, file->file, file->bytes);
    free(file->file);
    free(file);



    // /************************/
    // // LEITURA LEITURA LEITURA LEITURA LEITURA
    // // LEITURA LEITURA LEITURA LEITURA LEITURA
    // // LEITURA LEITURA LEITURA LEITURA LEITURA

    //superbloco
    Superblock sb2;
    file_t* tmp = readBlock(sb, 0, sizeof(Superblock));
    memcpy(&sb2, tmp, sizeof(Superblock));
    printf("tipo %s, fatpos %d, rootpos %d, blocksize %d, blockQtde %d\n", sb2.tipo, sb2.fatPos, sb2.rootPos, sb2.blockSize, sb2.blockQtde);
    free(tmp);
    
    //fat
    //Fat* fat2 = (Fat*)malloc(sizeof(Fat)*sb2.blockQtde);
    Fat fat2[sb2.blockQtde];
    tmp = readBlock(sb, sb2.fatPos, sizeof(Fat)*sb2.blockQtde);
    memcpy(&fat2, tmp, sizeof(Fat)*sb2.blockQtde);
    printf("fat: ");
    for(int i=0; i<sb2.blockQtde; i++){
        switch (fat2[i])
        {
        case FAT_L:
            printf("L, ");
            break;
        case FAT_F:
            printf("F, ");
            break;
        case FAT_R:
            printf("R, ");
            break;
        case FAT_B:
            printf("B, ");
            break;
        
        default:
            break;
        }
        //printf("%d, ", fat2[i]);
    }
    printf("\n");
    free(tmp);

    //root
    DirMeta meta2;
    Entry entries[2];
    tmp = readBlock(sb, sb2.rootPos, sb2.blockSize);
    memcpy(&meta2, tmp, sizeof(DirMeta));
    memcpy(&entries, tmp+sizeof(DirMeta), sizeof(Entry)*2);
    printf("meta %s fblock %d, bytes %d, entryQtde %d // entry1 %s type %s // entry2 %s type %s\n", meta2.name, meta2.firstBlock, meta2.bytes, meta2.entryQtde, entries[0].name, entries[0].type, entries[1].name, entries[1].type);
    free(tmp); 
}

// file_chunk* createDirectory(char* name, int freeBlock, DirMeta fatherMeta){
//     DirMeta meta;
//     char oi[500]; //WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
//     Entry entries[fatherMeta.entryQtde];
//     printf("end: %ld, %ld, %ld\n", &entries[0], &entries[1], &meta);
    
//     //meta
//     // strcpy(meta.name, name);
//     // printf("name %s\n", meta.name);SS
//     int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
//     strncpy(meta.name, name, nameSize); //copia no maximo 7 caracteres
//     meta.name[nameSize] = '\0'; //adiciona final de string ao final do nome.
    
//     meta.entryQtde = 2;    
//     meta.bytes = sizeof(DirMeta) + meta.entryQtde*sizeof(Entry);
//     meta.firstBlock = freeBlock;
    
//     // .
//     strcpy(entries[0].name, ".UM");
//     strcpy(entries[0].type, "dir");
//     entries[0].firstBlock = meta.firstBlock;

//     // ..
//     strcpy(entries[1].name, "..DOIS");
//     strcpy(entries[1].type, "dir");
//     entries[1].firstBlock = fatherMeta.firstBlock;

//     //Criando chunk com o file_t e controle de bytes
//     file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk));
//     fc->bytes = meta.bytes;

//     //Criando arquivo file_t
//     fc->file = (file_t*)malloc(meta.bytes*sizeof(char));
//     memcpy(fc->file, &meta, sizeof(DirMeta));
//     memcpy(fc->file+sizeof(DirMeta), &entries, sizeof(Entry)*meta.entryQtde);
//     return fc;
// }

// file_chunk* createDirectory(char* name, int freeBlock, DirMeta fatherMeta){
//     DirMeta meta;
//     // char oi[500]; //WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
//     Entry* entry1 = (Entry*)malloc(sizeof(Entry));//[fatherMeta.entryQtde];
//     Entry* entry2 = (Entry*)malloc(sizeof(Entry));//[fatherMeta.entryQtde];
//     // printf("end: %ld, %ld, %ld\n", &entries[0], &entries[1], &meta);
    
//     //meta
//     // strcpy(meta.name, name);
//     // printf("name %s\n", meta.name);SS
//     int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
//     strncpy(meta.name, name, nameSize); //copia no maximo 7 caracteres
//     meta.name[nameSize] = '\0'; //adiciona final de string ao final do nome.
    
//     meta.entryQtde = 2;    
//     meta.bytes = sizeof(DirMeta) + meta.entryQtde*sizeof(Entry);
//     meta.firstBlock = freeBlock;
    
//     // .
//     strcpy(entry1->name, ".");
//     strcpy(entry1->type, "dir");
//     entry1->firstBlock = meta.firstBlock;

//     // ..
//     strcpy(entry2->name, "..");
//     strcpy(entry2->type, "dir");
//     entry2->firstBlock = fatherMeta.firstBlock;

//     //Criando chunk com o file_t e controle de bytes
//     file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk));
//     fc->bytes = meta.bytes;

//     //Criando arquivo file_t
//     fc->file = (file_t*)malloc(meta.bytes*sizeof(char));
//     memcpy(fc->file, &meta, sizeof(DirMeta));
//     memcpy(fc->file+sizeof(DirMeta), entry1, sizeof(Entry));
//     memcpy(fc->file+sizeof(DirMeta)+sizeof(Entry), entry2, sizeof(Entry));
//     free(entry1);
//     free(entry2);
//     return fc;
// }


file_chunk* createDirectory(char* name, int freeBlock, DirMeta fatherMeta){
    DirMeta meta;
    // char oi[500]; //WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    Entry entry1, entry2;//[fatherMeta.entryQtde];
    printf("end: %ld, %ld, %ld\n", &entry1, &entry2, &meta);
    
    //meta
    // strcpy(meta.name, name);
    // printf("name %s\n", meta.name);SS
    int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
    strncpy(meta.name, name, nameSize); //copia no maximo 7 caracteres
    meta.name[nameSize] = '\0'; //adiciona final de string ao final do nome.
    
    meta.entryQtde = 2;    
    meta.bytes = sizeof(DirMeta) + meta.entryQtde*sizeof(Entry);
    meta.firstBlock = freeBlock;
    
    // .
    strcpy(entry1.name, ".");
    strcpy(entry1.type, "dir");
    entry1.firstBlock = meta.firstBlock;

    // ..
    strcpy(entry2.name, "..");
    strcpy(entry2.type, "dir");
    entry2.firstBlock = fatherMeta.firstBlock;

    //Criando chunk com o file_t e controle de bytes
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk));
    fc->bytes = meta.bytes;

    //Criando arquivo file_t
    fc->file = (file_t*)malloc(meta.bytes*sizeof(char));
    memcpy(fc->file, &meta, sizeof(DirMeta));
    memcpy(fc->file+sizeof(DirMeta), &entry1, sizeof(Entry));
    memcpy(fc->file+sizeof(DirMeta)+sizeof(Entry), &entry2, sizeof(Entry));
    // free(entry1);
    // free(entry2);
    return fc;
}


void writeBlock(Superblock sb, int blockNum, void* buffer, int bufferSize){
    if(blockNum >= sb.blockQtde) return;
    if(bufferSize <= 0 || bufferSize > sb.blockSize) return;

    FILE* dsc = fopen(DISCO, "r+");

    //ir para o bloco selecionado
    fseek(dsc, blockNum*sb.blockSize, SEEK_SET);

    //gravar no disco
    fwrite(buffer, sizeof(char), bufferSize, dsc);
    fclose(dsc);
}


file_t* readBlock(Superblock sb, int blockNum, int bufferSize){
    if(blockNum >= sb.blockQtde) return NULL;
    if(bufferSize <= 0 || bufferSize > sb.blockSize) return NULL;

    FILE* dsc = fopen(DISCO, "r");
    file_t* buffer = (file_t*)malloc(sizeof(char)*bufferSize);

    //ir para o bloco selecionado e ler os bytes
    fseek(dsc, blockNum*sb.blockSize, SEEK_SET);
    fread(buffer, sizeof(char), bufferSize, dsc);

    fclose(dsc);
    return buffer;
}

FileSystem* createFile(Superblock sb, file_chunk* bloco, int blockNum, char* nome){
    if(blockNum >= sb.blockQtde) return NULL; //sem espaco
        // criando file
        int ID = bloco->qtdFiles++;
        char * type = strtok(nome, ".");
        type = strtok(NULL, ".");

        FileSystem* novo = (FileSystem*) malloc(sizeof(FileSystem));
        novo->bloco = bloco;
        novo->blockNum = blockNum;
        novo->nome = nome;
        novo->type = type;
        novo->ID = ID;
      
        FILE *fp;
        fp  = fopen (nome, "w"); //path atual
        fp = fclose(fp);

        // atualizar fat
        //update()

       return novo;
 
};






// Fat fat2[sb.blockQtde];

//     dsc = fopen(DISCO, "r");
//     fseek(dsc, sb.fatPos*sb.blockSize, SEEK_SET);
//     // char oi[50];
//     fread(fat2, sizeof(Fat), sb.blockQtde, dsc);
//     // printf("oi %s %d\n", oi, 5);
//     fclose(dsc);
//     for(int i=0; i<blockQtde; i++){
//             printf("i %d, ", fat2[i]);
//         }





    // findFreeBlock();


    // converter diretorio de file_t para varias structs diferentes
    //
    // file_t* file = createDirectory("root", sb.rootPos, meta);
    //
    // DirMeta meta2;
    // Entry entries[2];
    // memcpy(&meta2, file, sizeof(meta2));
    // memcpy(entries, file+sizeof(meta2), sizeof(entries));
    //
    // printf("meta %s\nentry1 %s type %s\nentry2 %s type %s\n", meta2.name, entries[0].name, entries[0].type, entries[1].name, entries[1].type);
