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
void help(){
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
    for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
        if(strcmp(diretorioAtual->entries[i]->name, name) == 0){

            //se encontrei arquivo e eh um dir
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){

                int firstBlock = diretorioAtual->entries[i]->firstBlock;
                DirChunk* auxDir = facc_loadDir(sb, fat, firstBlock);

                if(auxDir->meta.entryQtde > 2){
                    printf("Nao eh possivel deletar '%s': Diretorio nao vazio.\n", name);
                
                }else{
                    facc_updateDirDel(sb, fat, diretorioAtual, i);
                }

                facc_unloadDirectory(auxDir);
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
    
    for(int i=0; i<diretorioAtual->meta.entryQtde; i++){ 
        if(strcmp(diretorioAtual->entries[i]->name, name) == 0){
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                int firstBlock = diretorioAtual->entries[i]->firstBlock;
                facc_unloadDirectory(diretorioAtual);
                novoDir = facc_loadDir(sb, fat, firstBlock);
                notFound = 0;
                break;
            }
        }
    }
    
    if(notFound == 1){
        printf("Diretorio nao encontrado\n");
        return diretorioAtual;
    }
    
    if(strcmp(name,"..")== 0){
        int size = strlen(pathing);
        while(size>=0){
            if(pathing[size] == '/'){
                pathing[size] = '\0';
                break;
            }
            size--;
        }
    }else if(strcmp(name, ".") == 0){
        //faz nada
    }else{
        strcat(pathing, "/");
        strcat(pathing, novoDir->meta.name);
    }
    //pwd path

    return novoDir;
}

void makeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name){
        //checar se ja existe diretorio com este nome
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strcmp(diretorioAtual->entries[i]->name, name) == 0){
                printf("Erro, ja existe um diretorio com este nome\n");
                return;
            }
        }

        //Encontro um bloco livre
        int blockNum = facc_findFreeBlock(sb, fat);
        if(blockNum < 0){
            printf("Erro, o disco esta cheio!\n");
            return;
        }
        
        //Crio um diretorio e guardo no disco
        file_chunk* newDir = createDirectory(name, blockNum, &diretorioAtual->meta);
        writeBlock(sb, blockNum, newDir->file, newDir->bytes);
        updateFat(sb, fat, blockNum, FAT_L);

        //criando a referencia pro diretorio
        Entry* ref = (Entry*)malloc(sizeof(Entry));

        int nameSize = strlen(name)>7 ? 7:strlen(name); //verifica se name tem mais que 7 caracteres
        strncpy(ref->name, name, nameSize); //copia no maximo 7 caracteres
        ref->name[nameSize] = '\0'; //adiciona final de string ao final do nome.
        
        ref->firstBlock = blockNum;
        strcpy(ref->type, "dir");        
        
        
        //crio a referencia (entry) do novoDir dentro do diretorioatual
        facc_updateDirAdd(sb, fat, diretorioAtual, ref);
}

void rmItem(DirChunk* diretorioAtual,Superblock* sb,char* name){
    

    //no diretorioatual tu tem q achar a entrada doq tu quer remover
    //ai tu vai na FAT, e faz o caminho que a lista encadeada de leva, setando todos pra FAT_F

    
    printf("rmItem");
//dirMeta (?)
}

void copyItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
    printf("copyItem");
//dirMeta (?)
}

void moveItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation){
    printf("moveItem");
}

char* createPathing(){
    char* path = (char*) malloc (sizeof(char)*250);
    strcpy(path,"~root");

    return path;
}

void listDirectory(DirChunk* diretorioAtual, char* name){
    //se nao ha segundo parametro
    if(name == NULL){
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                printf("%s\t", diretorioAtual->entries[i]->name);
            }else{
                printf("%s.%s\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
            }
        }
    
    //se o ultimo elemento for * mostra todos os arquivos com as iniciais
    }else if(name[strlen(name)-1] == '*') {
        for(int i=0; i<diretorioAtual->meta.entryQtde; i++){
            if(strncmp(diretorioAtual->entries[i]->name, name, strlen(name)-1) == 0){
                if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                    printf("%s\t", diretorioAtual->entries[i]->name);
                }else{
                    printf("%s.%s\t", diretorioAtual->entries[i]->name, diretorioAtual->entries[i]->type);
                }
            }
        }
    
    //se esta procurando um arquivo especifico
    }else{
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
                //se encontrou o arquivo, checar se eh dir
                if(strcmp(diretorioAtual->entries[i]->type, "dir") == 0){
                    printf("%s\t", diretorioAtual->entries[i]->name);
                    //chamar open dir e listar os diretorios dentro do diretorio "name"
                }else{
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
    printf("%s\n", pathing);
}

int format_dsc(int blockQtde){
    if(blockQtde < 4) return 0;
    
    facc_format(4096, blockQtde);
}
