/*
Pode ser util para o comando "mv"
    #include <fcntl.h>
  > int open(const char *path, int oflag, ... );
    https://pubs.opengroup.org/onlinepubs/009695399/functions/open.html

    The read() function shall attempt to read nbyte bytes from the file associated with the open file descriptor, fildes, into the buffer pointed to by buf. 
    #include <unistd.h>
  > ssize_t read(int fildes, void *buf, size_t nbyte);
    [XSI] [Option Start] ssize_t pread(int fildes, void *buf, size_t nbyte, off_t offset); [Option End]
    https://pubs.opengroup.org/onlinepubs/009695399/functions/read.html

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "commands.h"


/*
============================================================================================
========================================== help() ==========================================
============================================================================================
*/

void help(){ // Função que imprime a ajuda
    printf("Descricao dos comandos:\n\n");
 
    printf("cd - Acessa um diretorio\n");
    printf("\tcd <dir>\n\tcd /caminho/nome\n\n");

    printf("mkdir - Cria um diretorio\n");
    printf("\tmkdir <dir>\n\tmkdir /caminho/nome\n\n");

    printf("rm - Remove um diretorio/arquivo\n");
    printf("\trm <dir | arquivo>\n\trm /caminho/nome\n\n");

    printf("cp - Copia um diretorio/arquivo\n");
    printf("\tcp <dir | arquivo>\n\tcp /caminho/nome\n\n");

    printf("mv - Move Diretório/Arquivo.\n");
    printf("\tmv <dir | arquivo>\n\n");

    printf("ls - Lista o conteudo de um diretorio ou mostra as informacoes ddo arquivo\n");
    printf("\tls <dir | arquivo>\n\n");

    printf("pwd - Mostra o caminho do diretorio atual\n");
    printf("\tpwd\n\n");

    printf("format dsc - Formata o disco simulado\n");
    printf("\tformat dsc <num blocks>\n\n");

    printf("exp - Exporta um arquivo para o diretorio do programa.\n");
    printf("\texp <arquivo> <novo nome>\n\n");

    printf("print fat - Imprime a fat\n");
    printf("\tprint fat\n\n");
}

/*
=====================================================================================================
========================================== removeItem() ==========================================
=====================================================================================================
*/

void removeItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual,  char* name){
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0 ){
        printf("Nao e possivel excluir o diretorio de referencia '%s'.\n", name);
        return;
    }

    int notFound = 1;
    for(int i=0; i<diretorioAtual->meta.entryQtde; i++){// Percorre o diretorio
        if(strcmp(diretorioAtual->entries[i]->name, name) == 0){// Se encontrar o arquivo

            //se encontrei arquivo e eh um dir
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){

                int firstBlock = diretorioAtual->entries[i]->firstBlock;// pega o primeiro bloco do diretorio
                DirChunk* auxDir = facc_loadDir(sb, fat, firstBlock); // carrega o diretorio

                if(auxDir->meta.entryQtde > 2){ // Se o diretorio nao esta vazio
                    printf("Nao eh possivel deletar '%s': Diretorio nao vazio.\n", name);
                
                }else{ // Se o diretorio esta vazio
                    facc_updateDirDel(sb, fat, diretorioAtual, i); // remove o diretorio do diretorio atual
                }

                facc_unloadDirectory(auxDir); // desaloca o diretorio auxiliar
                notFound = 0;
                break;
            
            //se encontrei arquivo e eh type qualquer (.txt, etc...)
            }else{
                facc_updateDirDel(sb, fat, diretorioAtual, i);
                break;
            }
        }
    }
    
    if(notFound == 1){
        printf("Arquivo ou diretorio '%s' nao encontrado\n", name);
        return;
    }
}

/*
======================================================================================================
======================================= changeDirectory() =======================================
======================================================================================================
*/

DirChunk* changeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name, char* pathing){ 
    DirChunk* novoDir;
    int notFound = 1;
    for(int i=0; i<diretorioAtual->meta.entryQtde; i++){  // Percorre o diretorio
        if(strcmp(diretorioAtual->entries[i]->name, name) == 0){ // Se encontrar o arquivo
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){ // Se encontrar um diretorio (tipo dir)
                int firstBlock = diretorioAtual->entries[i]->firstBlock; // pega o primeiro bloco do diretorio
                facc_unloadDirectory(diretorioAtual); // desaloca o diretorio atual
                novoDir = facc_loadDir(sb, fat, firstBlock); // carrega o diretorio
                notFound = 0; 
                break;
            }
        }
    }
    if(notFound == 1){ // Se nao encontrar o diretorio
        printf("Diretorio nao encontrado\n"); // imprime mensagem de erro
        return diretorioAtual; // retorna o diretorio atual (proprio diretorio)
    }
    if(strcmp(name,"..")== 0){ // Se o diretorio for o diretorio pai
        int size = strlen(pathing); // pega o tamanho do caminho
        while(size>=0){ // percorre o caminho
            if(pathing[size] == '/'){ // se encontrar um '/'
                pathing[size] = '\0'; // corta o caminho
                break; // para de percorrer o caminho
            }
            size--; 
        }
    }else if(strcmp(name, ".") == 0){ // Se o diretorio for o diretorio atual
        //faz nada
    }else{ 
        strcat(pathing, "/"); // concatena o caminho com "/"
        strcat(pathing, novoDir->meta.name); // concatena o caminho com o nome do diretorio
    }
    //pwd path
    return novoDir; // retorna o novo diretorio
}

/*
===================================================================================================
======================================== makeDirectory() ========================================
===================================================================================================
*/

void makeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name){
        //checar se ja existe diretorio com este nome
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){ // Percorre o diretorio
            if(strcmp(diretorioAtual->entries[i]->name, name) == 0){ // Se encontrar o arquivo
                printf("Erro, ja existe um diretorio com este nome\n");
                return;
            }
        }

        //Encontro um bloco livre
        int blockNum = facc_findFreeBlock(sb, fat); // pega o primeiro bloco livre
        if(blockNum < 0){ // Se nao encontrar bloco livre
            printf("Erro, o disco esta cheio!\n");
            return;
        }
        
        //Crio um diretorio e guardo no disco
        file_chunk* newDir = createDirectory(name, blockNum, &diretorioAtual->meta); // cria um diretorio
        writeBlock(sb, blockNum, newDir->file, newDir->bytes); // escreve o diretorio no disco
        updateFat(sb, fat, blockNum, FAT_L); // atualiza o FAT

        //criando a referencia pro diretorio
        Entry* ref = (Entry*)malloc(sizeof(Entry)); // cria um novo entry

        int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
        strncpy(ref->name, name, nameSize); //copia no maximo 7 caracteres
        ref->name[nameSize] = '\0'; //adiciona final de string ao final do nome.
        
        ref->firstBlock = blockNum; // atribui o bloco do diretorio
        strcpy(ref->type, "dir");        // atribui o tipo do diretorio
        
        //crio a referencia (entry) do novoDir dentro do diretorioatual
        facc_updateDirAdd(sb, fat, diretorioAtual, ref); 
}

/*
=======================================================================================================
============================================== moveItem() ==============================================
=======================================================================================================
*/

void moveItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination){
    int sourceEntry = -1;
    
    //checar se existe uma entrada com este nome
    int entryFound = -1;
    for(int i=0; i<diretorioAtual->meta.entryQtde; i++){ // Percorre o diretorio
        if(strcmp(diretorioAtual->entries[i]->name, source) == 0){ // Se encontrar o arquivo
            entryFound = i;
            sourceEntry = i;
            break;
        }
    }

    //se nao existir uma entrada retorna erro
    if(entryFound == -1){
        printf("Erro, nao existe um arquivo com este nome\n");
        return;
    }

    // >>>>>>>>>>>>>>>>> passo 1 <<<<<<<<<<<<<<<<<
    // >>>>>>>>>>>>>>>>> passo 1 <<<<<<<<<<<<<<<<<
    //Separar em novo nome e encontrar o proximo diretorio
    char path[250];
    char newName[8];

    //encontrar novo path
    strcpy(path, destination);
    int size = strlen(path);
    while(size>=0){ // percorre o caminho
        if(path[size] == '/'){ // se encontrar um '/'
            path[size] = '\0'; // corta o caminho
            break; // para de percorrer o caminho
        }
        size--; 
    }
    if(strlen(path) == strlen(destination)){ //se nao houver nenhum '/' eh pra renomear, portanto path eh vazio
        path[0] = '\0';
    }

    //encontrar o nome do arquivo
    int nameIndex = 0;
    for(int i=size+1; i<strlen(destination); i++){
        newName[nameIndex] = destination[i];
        nameIndex++;
    }
    newName[nameIndex] = '\0';
    newName[7] = '\0'; //limitando em 8 caracteres.

    // >>>>>>>>>>>>>>>>> passo 2 <<<<<<<<<<<<<<<<<
    // >>>>>>>>>>>>>>>>> passo 2 <<<<<<<<<<<<<<<<<
    // se nao existir path, ele entra aqui e renomeia
    if(strlen(path) < 1){

        // > mudando nome no meta do arquivo
        if(strcmp(diretorioAtual->entries[sourceEntry]->type, "dir") == 0){
            DirChunk* renameDir = facc_loadDir(sb, fat, diretorioAtual->entries[sourceEntry]->firstBlock);
            strcpy(renameDir->meta.name, newName);
            updateDirectory(sb, fat, renameDir);
            facc_unloadDirectory(renameDir);
        }

        // > mudando o nome na entry
        strcpy(diretorioAtual->entries[sourceEntry]->name, newName);
        updateDirectory(sb, fat, diretorioAtual);
        


    // se existir path, ele percorre o path procurando o ultimo diretorio
    }else{
        DirChunk* dir = facc_loadDir(sb, fat, diretorioAtual->meta.firstBlock);
        int firstBlock;

        char* token = strtok(path, "/");

        if(strcmp(token, source) == 0){
            printf("Erro, nao eh possivel mover um arquivo para dentro dele mesmo. (%s > %s)\n", source, token);
            return;
        }

        while(token != NULL){
            //verificar se o diretorio existe
            entryFound = -1;
            for(int i=0; i<dir->meta.entryQtde; i++){ // Percorre o diretorio
                if(strcmp(dir->entries[i]->name, token) == 0){ // Se encontrar um arquivo
                    if(strcmp(dir->entries[i]->type, "dir") == 0){  //se o arquivo for dir
                        entryFound = i;
                    }
                }
            }
            if(entryFound == -1){
                printf("Erro, diretorio de destino '%s' nao existe.\n", token);
                return;
            }

            //se o diretorio existir:
            firstBlock = dir->entries[entryFound]->firstBlock;
            if(dir != diretorioAtual) facc_unloadDirectory(dir);
            dir = facc_loadDir(sb, fat, firstBlock);

            token = strtok(NULL, "/");
        }
        
        // >>>>>>>>>>>>>>>>> passo 3 <<<<<<<<<<<<<<<<<
        // >>>>>>>>>>>>>>>>> passo 3 <<<<<<<<<<<<<<<<<

        //verificar se ja existe um arquivo com o mesmo nome no diretorio destino
        for(int i=0; i<dir->meta.entryQtde; i++){ // Percorre o diretorio
            if(strcmp(dir->entries[i]->name, newName) == 0){ // Se encontrar um arquivo
                printf("Erro, ja existe um arquivo com o nome '%s' no diretorio destino '%s'\n", newName, dir->entries[i]->name);
                return;
            }
        }

        //mudar o diretorio para o novo nome
        // > mudando na entry diretorio pai
        strcpy(diretorioAtual->entries[sourceEntry]->name, newName);

        // > mudando nome no meta do arquivo
        if(strcmp(diretorioAtual->entries[sourceEntry]->type, "dir") == 0){
            DirChunk* renameDir = facc_loadDir(sb, fat, diretorioAtual->entries[sourceEntry]->firstBlock);
            strcpy(renameDir->meta.name, newName);
            renameDir->entries[1]->firstBlock = dir->meta.firstBlock;
            updateDirectory(sb, fat, renameDir);
            facc_unloadDirectory(renameDir);
        }
        
        // crio a referencia (entry) do dir que estou movendo para o dir que sera o novo pai
        Entry* ref = (Entry*)malloc(sizeof(Entry));
        memcpy(ref, diretorioAtual->entries[sourceEntry], sizeof(Entry));
        
        //deletar a entrada sourceEntry no diretorioAtual;
        facc_updateDirDelEntry(sb, fat, diretorioAtual, sourceEntry);
        
        //adicionar a entrada sourceEntry no diretorioDestino;
        facc_updateDirAdd(sb, fat, dir, ref);
        // updateFat(sb, fat, )

        //tirar da memoria o diretorio destino
        facc_unloadDirectory(dir);
    }
}

/*
=====================================================================================================
========================================== createPathing() ==========================================
=====================================================================================================
*/

char* createPathing(){
    char* path = (char*) malloc (sizeof(char)*250); //aloca memoria para o path
    strcpy(path,"~root"); //copia o path inicial

    return path; //retorna o path
}

/*
=======================================================================================================
=========================================== listDirectory() ===========================================
=======================================================================================================
*/

void listDirectory(DirChunk* diretorioAtual, char* name){
   
    if(name == NULL){   //se nao ha segundo parametro
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){ 
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                printf("%s\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->firstBlock);
            }else{
                printf("%s(%s)\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
            }
        }
    
    }else if(name[strlen(name)-1] == '*') {    //se o ultimo elemento for * mostra todos os arquivos com as iniciais
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strncmp(diretorioAtual->entries[i]->name, name, strlen(name)-1) == 0){
                if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                    printf("%s\t", diretorioAtual->entries[i]->name);
                }else{
                    printf("%s(%s)\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
                }
            }
        }
    
    }else{    //se esta procurando um arquivo especifico
        int notFound = 1;
        
        //se name = ..DOIS/musicas/eletronica
        //char aux; e passo pro aux tudo antes do / e retiro essa palavra do name
        //aux = ..DOIS
        //name = musicas/eletronica

        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){         
            //if(strcmp(diretorioAtual->entries[i]->name, aux) == 0){
            //fazer a comparacao do entry[i] com o aux
            //verificar se name esta vazio, se estiver eh pq esta eh a ultima recursao
            //se for a ultima recursao, eu mostro todos os itens do diretorio
            //se nao for a ultima recursao, eu chamo o openDir com o diretorio encontrado
            //e chamo listDirectory com name = name
            
            if(strcmp(diretorioAtual->entries[i]->name, name) == 0){
                if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){    //se encontrou o arquivo, checar se eh dir
                    printf("%s\t", diretorioAtual->entries[i]->name);
                    //chamar open dir e listar os diretorios dentro do diretorio "name"
                }else{    //se não eh diretorio, lista as informacoes do arquivo (nome)
                    printf("%s(%s)\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
                }
                notFound = 0;
            }
        }
        
        if(notFound == 1){
            printf("Arquivo ou Diretorio inexistente.");
        }
    }
    printf("\n");
}

/*
=============================================================================
================================ showPath() ================================
=============================================================================
*/

void showPath(char* pathing){
    printf("%s\n", pathing); //mostra o path
}

/*
===============================================================================
================================ format_dsc() ================================
===============================================================================
*/

void format_dsc(int blockSize, int blockQtde){
    if(blockQtde < 4){ //se o numero de blocos for menor que 4, nao eh possivel formatar
        printf("Entrada inválida, a quantidade de blocos deve possuir ao menos 4 blocos (sb, fat, root, data#1)\n");
        return;
    }
    
    facc_format(blockSize, blockQtde); //formata o disco
}

/*
===============================================================================
========================== copyItem() =========================================
===============================================================================
*/

void copyItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination){  
    int entryFound = -1;
    int sourceEntry = 0;

    //se nao estiver buscando arquivos externos:
    if(strncmp(destination, "dsc", 3) != 0){
        //checar se existe uma entrada com este nome
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){ // Percorre o diretorio
            if(strcmp(diretorioAtual->entries[i]->name, source) == 0){ // Se encontrar o arquivo
                entryFound = i;
                sourceEntry = i;
                break;
            }
        }

        //se nao existir uma entrada retorna erro
        if(entryFound == -1){
            printf("Erro, nao existe um arquivo com este nome\n");
            return;
        }
    }

    // >>>>>>>>>>>>>>>>> passo 1 <<<<<<<<<<<<<<<<<
    // >>>>>>>>>>>>>>>>> passo 1 <<<<<<<<<<<<<<<<<
    //Separar em novo nome e encontrar o proximo diretorio
    char path[250];
    char newName[50];
    char fileType[50];

    //encontrar novo path
    strcpy(path, destination);
    int size = strlen(path);
    while(size>=0){ // percorre o caminho
        if(path[size] == '/'){ // se encontrar um '/'
            path[size] = '\0'; // corta o caminho
            break; // para de percorrer o caminho
        }
        size--; 
    }
    if(strlen(path) == strlen(destination)){ //se nao houver nenhum '/' eh pra renomear, portanto path eh vazio
        path[0] = '\0';
    }

    //encontrar o nome do arquivo
    int nameIndex = 0;
    for(int i=size+1; i<strlen(destination); i++){ //percorre o caminho
        newName[nameIndex] = destination[i]; //copia o nome do arquivo
        nameIndex++; //incrementa o indice do nome
    }

    //encontrar a extensao do arquivo
    int flag = 0;
    int ext = 0;
    for(int i=0; i<strlen(newName); i++){ //percorre o nome do arquivo
        if(flag == 1){ //se flag for 1, eh pra pegar a extensao
            fileType[ext] = newName[i]; //copia a extensao do arquivo
            ext++; //incrementa o indice da extensao
        }
        if(newName[i] == '.' && i != 0){ //se encontrar um ponto e nao for o primeiro caractere
            nameIndex = i; //salva o indice do ponto
            flag = 1;
        }
    }

    fileType[ext] = '\0'; //termina a extensao do arquivo
    fileType[3] = '\0'; //termina a extensao do arquivo

    newName[nameIndex] = '\0'; //termina o nome do arquivo
    newName[7] = '\0'; //limitando em 8 caracteres.

    // >>>>>>>>>>>>>>>>> passo 2 <<<<<<<<<<<<<<<<<
    // >>>>>>>>>>>>>>>>> passo 2 <<<<<<<<<<<<<<<<<

    // se nao existir path, ele entra aqui e duplica
    if(strlen(path) < 1){

        //verificar se ja existe um arquivo com o mesmo nome no diretorio destino
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){ // Percorre o diretorio
            if(strcmp(diretorioAtual->entries[i]->name, newName) == 0){ // Se encontrar um arquivo
                printf("Erro, ja existe um arquivo com o nome '%s' no diretorio destino '%s'\n", newName, diretorioAtual->meta.name);
                return;
            }
        }

        //verificar se o arquivo que esta sendo copiado eh um diretorio e esta vazio
        if(strcmp(diretorioAtual->entries[sourceEntry]->type, "dir") == 0){
            DirChunk* auxdir = facc_loadDir(sb, fat, diretorioAtual->entries[sourceEntry]->firstBlock);
            
            //verifica se o diretorio ta vazio
            if(auxdir->meta.entryQtde > 2){
                printf("Erro: Nao eh possivel copiar um diretorio nao vazio.\n");
                facc_unloadDirectory(auxdir);
                return;
            }
            
            //caso nao esteja vazio
            makeDirectory(sb, fat, diretorioAtual, newName);
            facc_unloadDirectory(auxdir);

        //caso nao seja um diretorio
        }else{
            //openFile;
            int freeBlock = facc_findFreeBlock(sb, fat);
            file_chunk* fc = openFile(sb, fat, diretorioAtual->entries[sourceEntry]);
            saveFile(sb, fat, fc, freeBlock);

            //createMeta
            Entry* ref = (Entry*)malloc(sizeof(Entry));
            strcpy(ref->name, newName);
            strcpy(ref->type, diretorioAtual->entries[sourceEntry]->type);
            ref->firstBlock = freeBlock;
            ref->bytes = fc->bytes;
            facc_updateDirAdd(sb, fat, diretorioAtual, ref);

            //atualizar o diretorio atual
            // int firstBlock = diretorioAtual->meta.firstBlock;
            // facc_unloadDirectory(diretorioAtual);
            // DirChunk* dir = facc_loadDir(sb, fat, firstBlock);
            // *diretorioAtual = *dir;
            
            free(fc->file);
            free(fc);
        }


    // se existir path, ele percorre o path procurando o ultimo diretorio
    }else{
        DirChunk* dir = facc_loadDir(sb, fat, diretorioAtual->meta.firstBlock);
        int firstBlock;

        char* token = strtok(path, "/");

        //verificar se eh pra trazer um arquivo externo
        //verificar se eh pra trazer um arquivo externo
        //verificar se eh pra trazer um arquivo externo
        //verificar se eh pra trazer um arquivo externo
        if(strcmp(token, "dsc") == 0){
            //verificar searquivo existe
            if(access(source, F_OK) != 0){
                printf("Erro, nao foi possivel encontrar no um arquivo '%s' no sistema real de arquivos.\n", source);
                return;
            }

            //remover 'dsc'
            token = strtok(NULL, "/");

            while(token != NULL){
                //verificar se o diretorio existe
                entryFound = -1;
                for(int i=0; i<dir->meta.entryQtde; i++){ // Percorre o diretorio
                    if(strcmp(dir->entries[i]->name, token) == 0){ // Se encontrar um arquivo
                        if(strcmp(dir->entries[i]->type, "dir") == 0){  //se o arquivo for dir
                            entryFound = i;
                        }
                    }
                }
                if(entryFound == -1){
                    printf("Erro, diretorio de destino '%s' nao existe.\n", token);
                    return;
                }

                //se o diretorio existir:
                firstBlock = dir->entries[entryFound]->firstBlock;
                if(dir != diretorioAtual) facc_unloadDirectory(dir);
                dir = facc_loadDir(sb, fat, firstBlock);

                token = strtok(NULL, "/");
            }

            //verificar se ja existe um arquivo com o mesmo nome no diretorio destino
            for(int i=0; i<dir->meta.entryQtde; i++){ // Percorre o diretorio
                if(strcmp(dir->entries[i]->name, newName) == 0){ // Se encontrar um arquivo
                    printf("Erro, ja existe um arquivo com o nome '%s' no diretorio destino '%s'\n", newName, dir->meta.name);
                    return;
                }
            }

            //######
            //carregar arquivo e fazer um file_chunk* dele.
            file_chunk* fc = importFile(source);

            //se deu certo
            if(fc != NULL){
                //guardar arquivo em um bloco
                int freeBlock = facc_findFreeBlock(sb, fat);
                saveFile(sb, fat, fc, freeBlock);

                //criar entrada no diratual;
                Entry* ref = (Entry*)malloc(sizeof(Entry));
                strcpy(ref->name, newName);
                if(strlen(fileType) > 0) strcpy(ref->type, fileType);
                else strcpy(ref->type, "file");
                ref->bytes = fc->bytes;
                ref->firstBlock = freeBlock;

                //adicionar entrada no dir atual;
                facc_updateDirAdd(sb, fat, dir, ref);

                if(dir->meta.firstBlock == diretorioAtual->meta.firstBlock){
                   
                    int dirBlock = dir->meta.firstBlock;
                    facc_unloadDirectory(diretorioAtual);
                    diretorioAtual = facc_loadDir(sb, fat, dirBlock);
                }

                //del file_chunk
                facc_unloadDirectory(dir);
                free(fc->file);
                free(fc);
            }


        //caso seja apenas pra copiar um arquivo interno
        //caso seja apenas pra copiar um arquivo interno
        //caso seja apenas pra copiar um arquivo interno
        //caso seja apenas pra copiar um arquivo interno
        }else{
            if(strcmp(token, source) == 0){
                printf("Erro, nao eh possivel copiar um arquivo para dentro dele mesmo. (%s > %s)\n", source, token);
                return;
            }

            while(token != NULL){
                //verificar se o diretorio existe
                entryFound = -1;
                for(int i=0; i<dir->meta.entryQtde; i++){ // Percorre o diretorio
                    if(strcmp(dir->entries[i]->name, token) == 0){ // Se encontrar um arquivo
                        if(strcmp(dir->entries[i]->type, "dir") == 0){  //se o arquivo for dir
                            entryFound = i;
                        }
                    }
                }
                if(entryFound == -1){
                    printf("Erro, diretorio de destino '%s' nao existe.\n", token);
                    return;
                }

                //se o diretorio existir:
                firstBlock = dir->entries[entryFound]->firstBlock;
                if(dir != diretorioAtual) facc_unloadDirectory(dir);
                dir = facc_loadDir(sb, fat, firstBlock);

                token = strtok(NULL, "/");
            }
            
            // printf("diretorio %s, entryqtde %d\n", dir->meta.name, dir->meta.entryQtde);

            // >>>>>>>>>>>>>>>>> passo 3 <<<<<<<<<<<<<<<<<
            // >>>>>>>>>>>>>>>>> passo 3 <<<<<<<<<<<<<<<<<

            //verificar se ja existe um arquivo com o mesmo nome no diretorio destino
            for(int i=0; i<dir->meta.entryQtde; i++){ // Percorre o diretorio
                if(strcmp(dir->entries[i]->name, newName) == 0){ // Se encontrar um arquivo
                    printf("Erro, ja existe um arquivo com o nome '%s' no diretorio destino '%s'\n", newName, dir->entries[i]->name);
                    return;
                }
            }

            //verificar se o arquivo que esta sendo copiado eh um diretorio e esta vazio
            if(strcmp(diretorioAtual->entries[sourceEntry]->type, "dir") == 0){
                DirChunk* auxdir = facc_loadDir(sb, fat, diretorioAtual->entries[sourceEntry]->firstBlock);
                
                //verifica se o diretorio ta vazio
                if(auxdir->meta.entryQtde > 2){
                    printf("Erro: Nao eh possivel copiar um diretorio nao vazio.\n");
                    facc_unloadDirectory(auxdir);
                    return;
                }
                
                //caso nao esteja vazio
                makeDirectory(sb, fat, dir, newName);
                facc_unloadDirectory(auxdir);

            //caso nao seja um diretorio
            }else{
                //openFile;
                int freeBlock = facc_findFreeBlock(sb, fat);
                file_chunk* fc = openFile(sb, fat, dir->entries[sourceEntry]);
                saveFile(sb, fat, fc, freeBlock);

                //createMeta
                Entry* ref = (Entry*)malloc(sizeof(Entry));
                strcpy(ref->name, newName);
                strcpy(ref->type, dir->entries[sourceEntry]->type);
                ref->firstBlock = freeBlock;
                ref->bytes = fc->bytes;
                facc_updateDirAdd(sb, fat, dir, ref);

                //tirar da memoria o diretorio destino
                facc_unloadDirectory(dir);
                free(fc->file);
                free(fc);
            }
        }
    }
}


/*
===============================================================================
========================== exportItem() =========================================
===============================================================================
*/

void exportItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination){
    if(access(destination, F_OK) == 0){
        printf("Erro, ja existe um arquivo chamado '%s' no diretorio do programa.\n");
        return;
    }

    int entryIndex = 0;
    int notFound = 1;
    //buscando o arquivo no diretorio
    for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
        //achou o arquivo
        if(strcmp(diretorioAtual->entries[i]->name, source) == 0){
            //se for diretorio
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                printf("Erro, nao eh possivel exportar um diretorio\n");
                return;
            }

            //se encontrou um arquivo
            notFound = 0;
            entryIndex = i;
            break;
        }
    }
    if(notFound == 1){
        printf("Erro, arquivo '%s' nao existe no diretorio atual.\n", source);
        return;
    }

    //carregando o arquivo na memoria
    file_chunk* fc = openFile(sb, fat, diretorioAtual->entries[entryIndex]);

    //exportanto arquivo
    FILE* file = fopen(destination, "wb");
    fwrite(fc->file, sizeof(char), fc->bytes, file);
    fclose(file);
    
    free(fc->file);
    free(fc);
}

/*
===============================================================================
========================== printFat() =========================================
===============================================================================
*/

void printFat(Superblock* sb, Fat* fat){
    printf("FAT => ");
    for(int i = 0; i < sb->blockQtde; i++){
        if(fat[i] == FAT_L)         printf("L");
        else if(fat[i] == FAT_F)    printf("F");
        else if(fat[i] == FAT_R)    printf("R");
        else if(fat[i] == FAT_B)    printf("B");
        else                        printf("%d", fat[i]);
        if(i != sb->blockQtde-1)    printf(", ");
    }
    printf("\n");
}