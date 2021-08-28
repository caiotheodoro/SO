void help();

void listenCommand(char* command,DirChunk* diretorioAtual,Superblock* sb);

char** split(char* command);
void changeDirectory(DirChunk* diretorioAtual,Superblock* sb,char* name,int blockNum);
void makeDirectory(DirChunk* diretorioAtual,Superblock* sb,char* name, int blockNum);
void rmItem(DirChunk* diretorioAtual,Superblock* sb,char* name);
void copyItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation);
void moveItem(DirChunk* diretorioAtual,Superblock* sb,char* source,char* destation);
void listDirectory(DirChunk* diretorioAtual,Superblock* sb);
void showPath(DirChunk* diretorioAtual,Superblock* sb);
