#include "fatacc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
/* ========================== funcoes internas ========================= */
// file_chunk* createDirectory(char* name, int freeBlock, DirMeta* fatherMeta);
file_chunk* openDir(Superblock* sb, Fat* fat, int firstBlock);

// void writeBlock(Superblock* sb, int blockNum, void* buffer, int bufferSize);
file_t* readBlock(Superblock* sb, int blockNum, int bufferSize);

// void updateFat(Superblock* sb, Fat* fat, int pos, int nextPos);
// void updateDirectory(Superblock* sb, Fat* fat, DirChunk* directory);

void deleteFile(Superblock* sb, Fat* fat, int firstBlock);
// void saveFile(Superblock* sb, Fat* fat, file_chunk* fc, int block);
// file_chunk* openFile(Superblock* sb, Fat* fat, Entry* meta);
/* ===================================================================== */

/*
=======================================================================================================
============================== facc_format(int blockSize, int blockQtde) ==============================
=======================================================================================================
*/

// Formata o disco, deixando-o com um superbloco, fat e um diretorio root
void facc_format(int blockSize, int blockQtde){
    /* criar disco */
    FILE* dsc = fopen(DISCO, "wb"); // cria o disco
    char* tmp2 = (char*)calloc(blockSize, sizeof(char)); // cria um buffer temporario
    for(int i=0; i<blockQtde; i++) fwrite(tmp2, 1, blockSize, dsc); // escreve o buffer no disco
    free(tmp2); // libera o buffer 
    fclose(dsc);     // fecha o disco


    /* criar superbloco */
    Superblock sb; 
    strcpy(sb.tipo, "fatacc"); // tipo do superbloco

    sb.fatPos = 1; // posicao do fat
    sb.rootPos = 2; // posicao do diretorio root
    sb.blockPos = 3; // posicao do primeiro bloco de dados
    
    sb.blockSize = blockSize; // tamanho do bloco
    sb.blockQtde = blockQtde; // quantidade de blocos

    writeBlock(&sb, 0, &sb, sizeof(Superblock)); // escreve o superbloco no disco


    /* fat */
    Fat* fat = (Fat*)malloc(sizeof(Fat) * sb.blockQtde); // cria o fat

    for(int i=0; i<3; i++){ // preenche o fat
        fat[i] = FAT_R; //reservados
    }

    for(int i=3; i<blockQtde; i++){
        fat[i] = FAT_F;     //livres
    }

    writeBlock(&sb, sb.fatPos, fat, sizeof(Fat)*sb.blockQtde); // escreve o fat no disco

    
    /* dirRoot */
    //cria o que seria o meta do diretorio pai para inserir na entry ".."
    DirMeta fatherMeta; 
    fatherMeta.firstBlock = sb.rootPos; // posicao do primeiro bloco do diretorio pai
    
    // 1 - encontrar posicao
    // -> por ser root, usara a posicao superbloco.rootPos

    // 2 - criar diretorio filho e guardar no disco
    file_chunk* file = createDirectory("root", sb.rootPos, &fatherMeta); // cria o diretorio root
    writeBlock(&sb, sb.rootPos, file->file, file->bytes); // escreve o diretorio root no disco
    updateFat(&sb, fat, sb.rootPos, FAT_L); // atualiza o fat

    // 3 - criar entry no diretorio pai e guardar no disco
    // -> nao ha diretorio pai pois este eh o root

    /* free */
    free(fat);     // libera o fat
    free(file->file); // libera o file_chunk
    free(file); // libera o file_chunk
}

/*
=======================================================================================================
======================================== facc_loadSuperblock() ========================================
=======================================================================================================
*/

Superblock* facc_loadSuperblock(){
    //superblock que sera retornado
    Superblock* sb = (Superblock*)malloc(sizeof(Superblock)); // cria o superbloco
    sb->blockSize = 512; //valores estaticos para poder usar no readBlock
    sb->blockQtde = 1;

    //read superblock do arquivo
    file_t* tmp = readBlock(sb, 0, sizeof(Superblock)); // le o superbloco do disco
    memcpy(sb, tmp, sizeof(Superblock)); // copia o superbloco para o retorno

    free(tmp); // libera o file_t
    return sb; // retorna o superbloco
}

/*
========================================================================================================
===================================== facc_loadFat(Superblock* sb) =====================================
========================================================================================================
*/

Fat* facc_loadFat(Superblock* sb){ // carrega o fat do disco
    //fat
    Fat* fat = (Fat*)calloc(sb->blockQtde, sizeof(Fat)); // cria o fat

    // Fat fat2[sb2.blockQtde];
    file_t* tmp = readBlock(sb, sb->fatPos, sizeof(Fat)*sb->blockQtde); // le o fat do disco
    memcpy(fat, tmp, sizeof(Fat)*sb->blockQtde); // copia o fat para o retorno

    free(tmp); // libera o file_t
    return fat; // retorna o fat
}

/*
========================================================================================================
======================== facc_loadDir(Superblock* sb, Fat* fat, int firstBlock) ========================
========================================================================================================
*/

DirChunk* facc_loadDir(Superblock* sb, Fat* fat, int firstBlock){ // carrega o diretorio do disco
    DirChunk* directory = (DirChunk*)malloc(sizeof(DirChunk)); // cria o diretorio

    //abrir arquivo
    file_chunk* fc = openDir(sb, fat, firstBlock); // abre o diretorio
    if(fc == NULL){ // se nao conseguiu abrir
        printf("Erro ao abrir diretorio\n"); // erro
        return NULL;
    }

    //alocando meta a partir do arquivo
    memcpy(&(directory->meta), fc->file, sizeof(DirMeta)); // copia o meta do diretorio para o retorno

    //criando vetor de entries
    directory->entries = (Entry**)malloc(directory->meta.entryQtde * sizeof(Entry*)); // aloca o vetor de entries

    //alocando entries a partir do arquivo
    for(int i=0; i<directory->meta.entryQtde; i++){ // para cada entry
        directory->entries[i] = (Entry*)malloc(sizeof(Entry)); // aloca o entry
        memcpy(directory->entries[i], fc->file + sizeof(DirMeta) + i*sizeof(Entry), sizeof(Entry)); // copia o entry para o retorno
    }

    free(fc->file); // libera o file_chunk
    free(fc); // libera o file_chunk
    return directory; // retorna o diretorio
}

/*
=======================================================================================================
================================= facc_unloadDirectory(DirChunk* dir) =================================
=======================================================================================================
*/

void facc_unloadDirectory(DirChunk* dir){ // desaloca o diretorio
    for(int i=0; i<dir->meta.entryQtde; i++){ // para cada entry
        free(dir->entries[i]); // libera o entry
    } 
    free(dir->entries); // libera o vetor de entries
    free(dir); // libera o diretorio
}

/*
========================================================================================================
============================= facc_findFreeBlock(Superblock* sb, Fat* fat) =============================
========================================================================================================
*/

int facc_findFreeBlock(Superblock* sb, Fat* fat){ // encontra um bloco livre
    for(int i=3; i < sb->blockQtde; i++){ // para cada bloco
        if(fat[i] == FAT_F){ // se for livre
            return i; // retorna a posicao
        }
    }
    return -1; // nao achou
}

/*
=======================================================================================================
========== facc_updateDirAdd(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, Entry* ref) ==========
=======================================================================================================
*/

void facc_updateDirAdd(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, Entry* ref){ // atualiza o diretorio apos adicionar um novo entry
     //criando vetor de entries
    Entry** newEntries = (Entry**)malloc(sizeof(Entry*) * (diretorioAtual->meta.entryQtde+1)); // aloca o vetor de entries
    
    //adiciono as entradas do dir->entries para o newEntries
    int i = 0;
    while(i<diretorioAtual->meta.entryQtde){ // para cada entry
        newEntries[i] = diretorioAtual->entries[i]; // copia o entry
        i++; // incrementa
    }

    //adiciono a nova entrada no newEntries
    newEntries[i] = ref;

    //coloco o NewEntries no lugar do antigo entries
    free(diretorioAtual->entries); 
    diretorioAtual->entries = newEntries;
    
    //coloco no meta que foi adicionado mais um diretorio
    diretorioAtual->meta.entryQtde++;

    updateDirectory(sb, fat, diretorioAtual); // atualiza o diretorio
}

/*
========================================================================================================
=========== facc_updateDirDel(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, int index) ===========
========================================================================================================
*/

void facc_updateDirDel(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, int index){ // atualiza o diretorio apos deletar um entry
    if(index < 2 || index > diretorioAtual->meta.entryQtde){ // se o index for invalido
        printf("Erro ao remover arquivo.\n"); // erro
        return;
    }

    //excluindo o arquivo.
    deleteFile(sb, fat, diretorioAtual->entries[index]->firstBlock); // exclui o arquivo

    //removendo a entrada.
    free(diretorioAtual->entries[index]); // libera o entry
    int ultimoEntry = diretorioAtual->meta.entryQtde - 1; // pega o ultimo entry


    diretorioAtual->entries[index] = diretorioAtual->entries[ultimoEntry]; // copia o ultimo entry para o index
    diretorioAtual->entries[ultimoEntry] = NULL; // libera o ultimo entry

    diretorioAtual->meta.entryQtde--; // diminui o entryQtde
     
    updateDirectory(sb, fat, diretorioAtual); // atualiza o diretorio
}

/*
=======================================================================================================
======== facc_updateDirDelEntry(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, int index) ========
=======================================================================================================
*/

void facc_updateDirDelEntry(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, int index){ // atualiza o diretorio apos deletar um entry
    if(index < 2 || index > diretorioAtual->meta.entryQtde){ // se o index for invalido
        printf("Erro ao remover arquivo.\n"); // erro
        return;
    }

    // funcao clone da anterior onde o arquivo nao eh deletado
    // //excluindo o arquivo.
    // deleteFile(sb, fat, diretorioAtual->entries[index]->firstBlock); // exclui o arquivo

    //removendo a entrada.
    free(diretorioAtual->entries[index]); // libera o entry
    int ultimoEntry = diretorioAtual->meta.entryQtde - 1; // pega o ultimo entry


    diretorioAtual->entries[index] = diretorioAtual->entries[ultimoEntry]; // copia o ultimo entry para o index
    diretorioAtual->entries[ultimoEntry] = NULL; // libera o ultimo entry

    diretorioAtual->meta.entryQtde--; // diminui o entryQtde
     
    updateDirectory(sb, fat, diretorioAtual); // atualiza o diretorio
}

/*
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
========================================================================================================
*/

/*
======================================================================================================
=================== updateDirectory(Superblock* sb, Fat* fat, DirChunk* directory) ===================
======================================================================================================
*/

void updateDirectory(Superblock* sb, Fat* fat, DirChunk* directory){ // atualiza o diretorio
    //remove o antigo da memoria
    int firstBlock = directory->meta.firstBlock; // pega o primeiro bloco
    deleteFile(sb, fat, firstBlock); // exclui o arquivo

    //Criando chunk com o file_t e controle de bytes
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk)); // cria o file_chunk
    fc->bytes = sizeof(DirMeta) + sizeof(Entry)*directory->meta.entryQtde; // define o tamanho do file_chunk

    //crio um file_t contendo as informacoes do DirChunk
    fc->file = (file_t*)malloc(fc->bytes * sizeof(char)); // aloca o file_t
    memcpy(fc->file, &directory->meta, sizeof(DirMeta)); // copia o meta do diretorio para o file_t
    for(int i=0; i<directory->meta.entryQtde; i++){ // para cada entry
        memcpy(fc->file+sizeof(DirMeta)+(i*sizeof(Entry)), directory->entries[i], sizeof(Entry)); // copia o entry para o file_t
    }
    
    //coloco no disco o file_t
    saveFile(sb, fat, fc, firstBlock); // salva o file_t no disco
}

/*
=======================================================================================================
==================== saveFile(Superblock* sb, Fat* fat, file_chunk* fc, int block) ====================
=======================================================================================================
*/
 
void saveFile(Superblock* sb, Fat* fat, file_chunk* fc, int block){ // salva o file_chunk no disco
    int bytes = fc->bytes; // pega o tamanho do file_chunk
    int i = 0; // contador
    int lastBlock = -1; // ultimo bloco

    while(bytes > 0){
        //escrevo o bloco na memoria
        int bufferSize = (bytes > sb->blockSize) ? sb->blockSize : bytes; // define o tamanho do buffer
        writeBlock(sb, block, fc->file + (i * sb->blockSize), bufferSize); // escreve o bloco na memoria
        
        // Faco com que o bloco anterior aponte para este bloco
        if(lastBlock != -1){
            updateFat(sb, fat, lastBlock, block); // atualiza o fat
        }

        //faco com que este bloco esteja ocupado e receba status de ultimo
        updateFat(sb, fat, block, FAT_L);

        //atualizo o ultimo bloco pra proxima iteracao
        lastBlock = block;
        
        //update das variaveis de controle
        i++;
        block = facc_findFreeBlock(sb, fat);
        bytes = bytes - sb->blockSize;
    }
}

/*
========================================================================================================
========================= deleteFile(Superblock* sb, Fat* fat, int firstBlock) =========================
========================================================================================================
*/

void deleteFile(Superblock* sb, Fat* fat, int firstBlock){
    
    int block = firstBlock; // primeiro bloco
    int aux = block; // auxiliar

    do{ // enquanto o bloco nao for o ultimo
        if(fat[block] == FAT_B || fat[block] == FAT_F) return; // erro

        block = fat[block]; // pega o proximo bloco
        
        fat[aux] = FAT_F; // libera o bloco
        aux = block; // atualiza o auxiliar
    } while(block != FAT_L); 
}

/*
=======================================================================================================
========================== openDir(Superblock* sb, Fat* fat, int firstBlock) ==========================
=======================================================================================================
*/

file_chunk* openDir(Superblock* sb, Fat* fat, int firstBlock){ 
    if(firstBlock < 0 || firstBlock >= sb->blockQtde) return NULL; // se o primeiro bloco for invalido

    int blocks_qtde = 0;
    int block = firstBlock;

    //guarda em blocks_qtde quantos blocos o arquivo utiliza
    do{  // enquanto o bloco nao for o ultimo
        if(fat[block] == FAT_B || fat[block] == FAT_F) return NULL; // erro
        blocks_qtde++; // incrementa o contador
        block = fat[block]; // pega o proximo bloco
    }while(block != FAT_L);
    
    //cria o file_chunk com a stream do arquivo e seus bytes para retornar
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk)); // cria o file_chunk
    fc->bytes = blocks_qtde * sb->blockSize; // define o tamanho do file_chunk
    fc->file = (file_t*)malloc(fc->bytes);  // aloca o file_t

    file_t* tmp; // auxiliar
    int i = 0; 
    block = firstBlock; // primeiro bloco

    //faz a leitura de cada bloco e adiciona na stream "file_t fc->file" 
    do{ // enquanto o bloco nao for o ultimo
        if(fat[block] == FAT_B || fat[block] == FAT_F) return NULL; // erro

        tmp = readBlock(sb, block, sb->blockSize); // le o bloco
        memcpy(fc->file + (i*sb->blockSize), tmp, sb->blockSize); // copia o bloco para o file_t
        free(tmp); // libera o bloco temporario

        i++; // incrementa o contador   
        block = fat[block]; // pega o proximo bloco
    }while(block != FAT_L);

    //retorno o file_chunk
    return fc;
}

/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////

file_chunk* openFile(Superblock* sb, Fat* fat, Entry* meta){ 
    if(meta->firstBlock < 0 || meta->firstBlock >= sb->blockQtde) return NULL; // se o primeiro bloco for invalido
     
    //cria o file_chunk com a stream do arquivo e seus bytes para retornar
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk)); // cria o file_chunk
    fc->bytes = meta->bytes; // define o tamanho do file_chunk
    fc->file = (file_t*)malloc(fc->bytes);  // aloca o file_t

    file_t* tmp; // auxiliar
    int i = 0; 
    int block = meta->firstBlock;
    int bytes = meta->bytes;

    //faz a leitura de cada bloco e adiciona na stream "file_t fc->file" 
    do{ // enquanto o bloco nao for o ultimo
        if(fat[block] == FAT_B || fat[block] == FAT_F) return NULL; // erro

        int readingBytes = (bytes > sb->blockSize) ? sb->blockSize : bytes;
        tmp = readBlock(sb, block, readingBytes); // le o bloco
        memcpy(fc->file + (i*sb->blockSize), tmp, readingBytes); // copia o bloco para o file_t
        free(tmp); // libera o bloco temporario

        i++; // incrementa o contador   
        block = fat[block]; // pega o proximo bloco
        bytes -= readingBytes;
    }while(block != FAT_L);

    if(bytes != 0){
        printf("Warning: O arquivo foi lido com mais ou menos bytes que o devido.\n");
    }

    //retorno o file_chunk
    return fc;
}

/*
=======================================================================================================
====================== updateFat(Superblock* sb, Fat* fat, int pos, int nextPos) ======================
=======================================================================================================
*/

void updateFat(Superblock* sb, Fat* fat, int pos, int nextPos){
    if(pos < 0 || pos >= sb->blockQtde) return; // se o primeiro bloco for invalido
    if(nextPos == FAT_R || nextPos == FAT_B) return; // se o proximo bloco for invalido
    if(pos == nextPos) return; // se o proximo bloco for o mesmo que o atual
    
    fat[pos] = nextPos;
    writeBlock(sb, sb->fatPos, fat, sizeof(Fat)*sb->blockQtde); // salva o fat no disco
}

/*
=======================================================================================================
=================== createDirectory(char* name, int freeBlock, DirMeta* fatherMeta) ===================
=======================================================================================================
*/

file_chunk* createDirectory(char* name, int freeBlock, DirMeta* fatherMeta){ 
    DirMeta meta; // cria o meta do diretorio
    Entry entries[2]; // cria o array de entries do diretorio
    
    //meta
    int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
    strncpy(meta.name, name, nameSize); //copia no maximo 7 caracteres
    meta.name[nameSize] = '\0'; //adiciona final de string ao final do nome.
    
    meta.entryQtde = 2;     // define a quantidade de entradas
    meta.bytes = sizeof(DirMeta) + meta.entryQtde*sizeof(Entry); // define o tamanho do meta
    meta.firstBlock = freeBlock; // define o primeiro bloco
    
    // .
    strcpy(entries[0].name, "."); // atual
    strcpy(entries[0].type, "dir"); // define como diretorio
    entries[0].firstBlock = meta.firstBlock; 

    // ..
    strcpy(entries[1].name, ".."); // bloco do pai
    strcpy(entries[1].type, "dir"); // define como diretorio
    entries[1].firstBlock = fatherMeta->firstBlock; 

    //Criando chunk com o file_t e controle de bytes
    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk)); // cria o file_chunk
    fc->bytes = meta.bytes;

    //Criando arquivo file_t
    fc->file = (file_t*)malloc(meta.bytes*sizeof(char)); // aloca o file_t
    memcpy(fc->file, &meta, sizeof(DirMeta)); // copia o meta para o file_t 
    memcpy(fc->file+sizeof(DirMeta), &entries, sizeof(Entry)*meta.entryQtde); // copia o array de entries para o file_t
    
    return fc;
}

/*
========================================================================================================
================ writeBlock(Superblock* sb, int blockNum, void* buffer, int bufferSize) ================
========================================================================================================
*/

void writeBlock(Superblock* sb, int blockNum, void* buffer, int bufferSize){
    if(blockNum >= sb->blockQtde) return;
    if(bufferSize <= 0 || bufferSize > sb->blockSize) return;

    FILE* dsc = fopen(DISCO, "rb+"); // abre o disco

    //ir para o bloco selecionado
    fseek(dsc, blockNum*sb->blockSize, SEEK_SET); 

    //gravar no disco
    fwrite(buffer, sizeof(char), bufferSize, dsc);
    fclose(dsc);
}

/*
=======================================================================================================
======================= readBlock(Superblock* sb, int blockNum, int bufferSize) =======================
=======================================================================================================
*/

file_t* readBlock(Superblock* sb, int blockNum, int bufferSize){
    if(blockNum >= sb->blockQtde) return NULL; // se o bloco for invalido
    if(bufferSize <= 0 || bufferSize > sb->blockSize) return NULL; // se o buffer for invalido

    FILE* dsc = fopen(DISCO, "rb"); // abre o disco
    file_t* buffer = (file_t*)malloc(sizeof(char)*bufferSize); // aloca o buffer

    //ir para o bloco selecionado e ler os bytes
    fseek(dsc, blockNum*sb->blockSize, SEEK_SET); // ir para o bloco selecionado
    fread(buffer, sizeof(char), bufferSize, dsc); // ler os bytes

    fclose(dsc); // fechar o disco
    return buffer; // retorna o buffer
}
/*
=======================================================================================================
======================= file_chunk* importFile(char* source) =======================
=======================================================================================================
*/


file_chunk* importFile(char* source){
    FILE* fp = fopen(source, "rb"); //abre o arquivo
    if(fp == NULL){ //se nao conseguir abrir o arquivo
        printf("Erro: Nao foi possivel importar o arquivo '%s'.\n", source);
        return NULL;
    }
 
    fseek(fp, 0, SEEK_END);//define o ponteiro do arquivo para o final
    int bytes = ftell(fp);//pega o tamanho do arquivo
    fseek(fp, 0, SEEK_SET);//define o ponteiro do arquivo para o inicio

    file_chunk* fc = (file_chunk*)malloc(sizeof(file_chunk));//aloca memoria para o file_chunk
    fc->file = (file_t*)malloc(sizeof(file_t) * bytes);//aloca memoria para o file_t
    fc->bytes = bytes;//atribui o tamanho do arquivo ao file_chunk

    fread(fc->file, sizeof(char), bytes, fp);
    fclose(fp);
    return fc;
}