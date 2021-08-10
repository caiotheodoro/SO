Como compilar:
    make

Como executar:
    ./main <N_THREADS> <proporcao leitores:escritor (x:1)>

Bibliotecas usadas:
    unistd.h - utilização de sleep
    time.h - utilizada no seed do srand, o qual foi utilizado para gerar as quantidades aleatórias de threads
    pthread.h - criação e utilização de threads
    semaphore.h - utilização de semáforos para resolução do problema

Exemplo de uso:
    terminal:
        make
        ./main 4 5 -> cria-se 4 threads com a chance de 1 em 5 para ser escritora
        apertar ctrl+c para parar o programa
