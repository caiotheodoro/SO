#include "fatacc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ========================== funcoes internas ========================= */

file_chunk* createDirectory(char* name, int freeBlock, DirMeta* fatherMeta);
void writeBlock(Superblock* sb, int blockNum, void* buffer, int bufferSize);
file_t* readBlock(Superblock* sb, int blockNum, int bufferSize);
void updateFat(Superblock* sb, Fat* fat, int pos, int nextPos);
file_chunk* openDir(Superblock* sb, Fat* fat, int firstBlock);

/* ===================================================================== */

// Formata o disco, deixando-o com um superbloco, fat e um diretorio root
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

    writeBlock(&sb, 0, &sb, sizeof(Superblock));


    /* fat */
    Fat* fat = (Fat*)malloc(sizeof(Fat) * sb.blockQtde);

    for(int i=0; i<3; i++){
        fat[i] = FAT_R; //reservados
    }

    for(int i=3; i<blockQtde; i++){
        fat[i] = FAT_F;
    }

    writeBlock(&sb, sb.fatPos, fat, sizeof(Fat)*sb.blockQtde);

    
    /* dirRoot */
    //cria o que seria o meta do diretorio pai para inserir na entry ".."
    DirMeta fatherMeta;
    fatherMeta.firstBlock = sb.rootPos;
    
    // 1 - encontrar posicao
    // -> por ser root, usara a posicao superbloco.rootPos

    // 2 - criar diretorio filho e guardar no disco
    file_chunk* file = createDirectory("root", sb.rootPos, &fatherMeta);
    writeBlock(&sb, sb.rootPos, file->file, file->bytes);
    updateFat(&sb, fat, sb.rootPos, FAT_L);

    // 3 - criar entry no diretorio pai e guardar no disco
    // -> nao ha diretorio pai pois este eh o root

    /* free */
    free(fat);    
    free(file->file);
    free(file);
}


Superblock* facc_loadSuperblock(int blockSize, int blockQtde){
    //superblock que sera retornado
    Superblock* sb = (Superblock*)malloc(sizeof(Superblock));
    sb->blockSize = blockSize;
    sb->blockQtde = blockQtde;

    //read superblock do arquivo
    file_t* tmp = readBlock(sb, 0, sizeof(Superblock));
    memcpy(sb, tmp, sizeof(Superblock));

    free(tmp);
    return sb;
}


Fat* facc_loadFat(Superblock* sb){
    //fat
    Fat* fat = (Fat*)calloc(sb->blockQtde, sizeof(Fat));

    // Fat fat2[sb2.blockQtde];
    file_t* tmp = readBlock(sb, sb->fatPos, sizeof(Fat)*sb->blockQtde);
    memcpy(fat, tmp, sizeof(Fat)*sb->blockQtde);

    free(tmp);
    return fat;
}

DirChunk* facc_loadRoot(Superblock* sb, Fat* fat){
    DirChunk* directory = (DirChunk*)malloc(sizeof(DirChunk));

    //abrir arquivo
    file_chunk* fc = openDir(sb, fat, sb->rootPos);
    if(fc == NULL){
        printf("Erro ao abrir diretorio\n");
        return NULL;
    }

    //alocando meta a partir do arquivo
    memcpy(&(directory->meta), fc->file, sizeof(DirMeta));

    //criando vetor de entries
    directory->entries = (Entry**)malloc(directory->meta.entryQtde * sizeof(Entry*));

    //alocando entries a partir do arquivo
    for(int i=0; i<directory->meta.entryQtde; i++){
        directory->entries[i] = (Entry*)malloc(sizeof(Entry));
        memcpy(directory->entries[i], fc->file + sizeof(DirMeta) + i*sizeof(Entry), sizeof(Entry));
    }

    free(fc->file);
    free(fc);
    return directory;
}


void facc_unloadDirectory(DirChunk* dir){
    for(int i=0; i<dir->meta.entryQtde; i++){
        free(dir->entries[i]);
    }
    free(dir->entries);
    free(dir);
}

/*
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
*/

file_chunk* openDir(Superblock* sb, Fat* fat, int firstBlock){
    if(firstBlock < 0 || firstBlock >= sb->blockQtde) return NULL;

    int blocks_qtde = 0;
    int block = firstBlock;

    //guarda em blocks_qtde quantos blocos o arquivo utiliza
    do{
        if(fat[block] == FAT_B || fat[block] == FAT_F) return NULL;
        blocks_qtde++;
        block = fat[block];
    }while(block != FAT_L);
    
    //cria o file_chunk com a stream do arquivo e seus bytes para retornar
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk));
    fc->bytes = blocks_qtde * sb->blockSize;
    fc->file = (file_t*)malloc(fc->bytes);

    file_t* tmp;
    int i = 0;
    block = firstBlock;

    //faz a leitura de cada bloco e adiciona na stream "file_t fc->file" 
    do{
        if(fat[block] == FAT_B || fat[block] == FAT_F) return NULL;

        tmp = readBlock(sb, block, sb->blockSize);
        memcpy(fc->file + (i*sb->blockSize), tmp, sb->blockSize);
        free(tmp);

        i++;
        block = fat[block];
    }while(block != FAT_L);

    //retorno o file_chunk
    return fc;
}


void updateFat(Superblock* sb, Fat* fat, int pos, int nextPos){
    if(pos < 0 || pos >= sb->blockQtde) return;
    if(nextPos == FAT_R || nextPos == FAT_B) return;
    if(pos == nextPos) return;
    
    fat[pos] = nextPos;
    writeBlock(sb, sb->fatPos, fat, sizeof(Fat)*sb->blockQtde);
}

file_chunk* createDirectory(char* name, int freeBlock, DirMeta* fatherMeta){
    DirMeta meta;
    Entry entries[2];
    
    //meta
    int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
    strncpy(meta.name, name, nameSize); //copia no maximo 7 caracteres
    meta.name[nameSize] = '\0'; //adiciona final de string ao final do nome.
    
    meta.entryQtde = 2;    
    meta.bytes = sizeof(DirMeta) + meta.entryQtde*sizeof(Entry);
    meta.firstBlock = freeBlock;
    
    // .
    strcpy(entries[0].name, ".UM");
    strcpy(entries[0].type, "dir");
    entries[0].firstBlock = meta.firstBlock;

    // ..
    strcpy(entries[1].name, "..DOIS");
    strcpy(entries[1].type, "dir");
    entries[1].firstBlock = fatherMeta->firstBlock;

    //Criando chunk com o file_t e controle de bytes
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk));
    fc->bytes = meta.bytes;

    //Criando arquivo file_t
    fc->file = (file_t*)malloc(meta.bytes*sizeof(char));
    memcpy(fc->file, &meta, sizeof(DirMeta));
    memcpy(fc->file+sizeof(DirMeta), &entries, sizeof(Entry)*meta.entryQtde);
    return fc;
}


void writeBlock(Superblock* sb, int blockNum, void* buffer, int bufferSize){
    if(blockNum >= sb->blockQtde) return;
    if(bufferSize <= 0 || bufferSize > sb->blockSize) return;

    FILE* dsc = fopen(DISCO, "r+");

    //ir para o bloco selecionado
    fseek(dsc, blockNum*sb->blockSize, SEEK_SET);

    //gravar no disco
    fwrite(buffer, sizeof(char), bufferSize, dsc);
    fclose(dsc);
}


file_t* readBlock(Superblock* sb, int blockNum, int bufferSize){
    if(blockNum >= sb->blockQtde) return NULL;
    if(bufferSize <= 0 || bufferSize > sb->blockSize) return NULL;

    FILE* dsc = fopen(DISCO, "r");
    file_t* buffer = (file_t*)malloc(sizeof(char)*bufferSize);

    //ir para o bloco selecionado e ler os bytes
    fseek(dsc, blockNum*sb->blockSize, SEEK_SET);
    fread(buffer, sizeof(char), bufferSize, dsc);

    fclose(dsc);
    return buffer;
}




// FileSystem* createFile(Superblock sb, file_chunk* bloco, int blockNum, char* nome){
//     if(blockNum >= sb.blockQtde) return NULL; //sem espaco
//         // criando file
//         int ID = bloco->qtdFiles++;
//         char * type = strtok(nome, ".");
//         type = strtok(NULL, ".");

//         FileSystem* novo = (FileSystem*) malloc(sizeof(FileSystem));
//         novo->bloco = bloco;
//         novo->blockNum = blockNum;
//         novo->nome = nome;
//         novo->type = type;
//         novo->ID = ID;
      
//         FILE *fp;
//         fp  = fopen (nome, "w"); //path atual
//         fp = fclose(fp);

//         // atualizar fat
//         //update()

//        return novo;
 
// };






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