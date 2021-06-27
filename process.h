
typedef struct Processo {
    int PID;
    struct Processo *pai; 
    struct Processo *filho; 
} Processo;

Processo* criaProcesso(int N);