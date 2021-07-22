CC=gcc -Wall

PROGS=mqueue_client mqueue_server

all: $(PROGS)

clean:
	rm -f $(PROGS)

mqueue_client:	mqueue_client.c
	$(CC) mqueue_client.c -o mqueue_client -lrt

mqueue_server:	mqueue_server.c
	$(CC) mqueue_server.c -o mqueue_server -lrt
