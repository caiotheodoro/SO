CC=gcc -Wall

PROGS=semaforo_prod_cons sem_ex1 sem_ex2 mutex

all: $(PROGS)

clean:
	rm -f $(PROGS)

semaforo_prod_cons: produtor_consumidor_semaforo.c
	$(CC) produtor_consumidor_semaforo.c -o semaforo_prod_cons -lpthread

sem_ex1: sem-ex-1.c
	$(CC) sem-ex-1.c -o sem_ex1 -lpthread

sem_ex2: sem-ex-2.c
	$(CC) sem-ex-2.c -o sem_ex2 -lpthread -lm

mutex: mutex_pthread.c
	$(CC) mutex_pthread.c -o mutex -lpthread
