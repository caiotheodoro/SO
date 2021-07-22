CC=gcc -Wall

PROGS=client server

all: $(PROGS)

clean:
	rm -f $(PROGS)

client:	client.c
	$(CC) client.c -o client

server:	server.c
	$(CC) server.c -o server
