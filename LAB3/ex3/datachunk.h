typedef struct dc_t {
    int** matrix;
    int r, c, ini, fim;
    float* array;
}dc_t;

dc_t* dc_criaData(int** matrix, int r, int c, int ini, int fim, float* array);
