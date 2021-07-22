CC=gcc -Wall

PROGS=client server

all: $(PROGS)

clean:
	rm -f $(PROGS)

server:	server.c
	$(CC) server.c -o server -lrt

client:	client.c
	$(CC) client.c -o client -lrt
