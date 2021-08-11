Como compilar:
    make

Como executar:
    ./main <N_de_Elfos>

Bibliotecas usadas:
    unistd.h - utilização de sleep
    pthread.h - criação e utilização de threads

Exemplo de uso:
    terminal:
        make
        ./main 10 -> cria-se 1 thread Noel, 9 threads Renas e 10 threads Elfos
        o programa sera finalizado automaticamente apos as 9 renas serem executadas.
