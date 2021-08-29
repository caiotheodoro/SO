#include "fatacc.h"


void help();
char* createPathing();
DirChunk* changeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name,char* pathing);
void makeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name);
void removeItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual,  char* name);
// void copyItem(DirChunk* diretorioAtual, Superblock* sb, char* source, char* destation);
// void moveItem(DirChunk* diretorioAtual, Superblock* sb, char* source, char* destation);
void listDirectory(DirChunk* diretorioAtual, char* name);
int format_dsc(int blockQtde);
void showPath(char* pathing);