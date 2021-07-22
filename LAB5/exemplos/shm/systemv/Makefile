CC=gcc -Wall

PROGS=shm_server shm_client

all: $(PROGS)

clean:
	rm -f $(PROGS)

shm_server:	shm_server.c
	$(CC) shm_server.c -o shm_server

shm_client:	shm_client.c
	$(CC) shm_client.c -o shm_client
