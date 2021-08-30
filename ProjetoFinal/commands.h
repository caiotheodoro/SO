#include "fatacc.h"


void help();
char* createPathing();
DirChunk* changeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name, char* pathing);
void makeDirectory(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name);
void removeItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* name);
void copyItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination);
void moveItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination);
void exportItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination);
void listDirectory(DirChunk* diretorioAtual, char* name);
void format_dsc(int blockSize, int blockQtde);
void showPath(char* pathing);
void exportItem(Superblock* sb, Fat* fat, DirChunk* diretorioAtual, char* source, char* destination);
void printFat(Superblock* sb, Fat* fat);