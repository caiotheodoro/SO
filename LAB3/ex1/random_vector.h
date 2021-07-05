typedef struct{
    int size;
    int* array;
} rv;

rv* rv_gerar(int tamanho, int modulo);
void rv_excluir(rv* vtr);