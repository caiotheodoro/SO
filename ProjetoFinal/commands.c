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

#include "commands.h"

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
}


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

void copyItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
    printf("copyItem");
//dirMeta (?)
}

void moveItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
    printf("moveItem");
}

char* createPathing(){
    char* path = (char*) malloc (sizeof(char)*250); //aloca memoria para o path
    strcpy(path,"~root"); //copia o path inicial

    return path; //retorna o path
}

void listDirectory(DirChunk* diretorioAtual, char* name){
   
    if(name == NULL){   //se nao ha segundo parametro
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                printf("%s\t", diretorioAtual->entries[i]->name);
            }else{
                printf("%s.%s\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
            }
        }
    
    }else if(name[strlen(name)-1] == '*') {    //se o ultimo elemento for * mostra todos os arquivos com as iniciais
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strncmp(diretorioAtual->entries[i]->name, name, strlen(name)-1) == 0){
                if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                    printf("%s\t", diretorioAtual->entries[i]->name);
                }else{
                    printf("%s.%s\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
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
                    printf("%s.%s\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
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

void showPath(char* pathing){
    printf("%s\n", pathing); //mostra o path
}

void format_dsc(int blockQtde){
    if(blockQtde < 4){ //se o numero de blocos for menor que 4, nao eh possivel formatar
        printf("Entrada inválida, a quantidade de blocos deve possuir ao menos 3 blocos (sb, fat, root)\n");
        return;
    }
    
    facc_format(4096, blockQtde); //formata o disco
}
