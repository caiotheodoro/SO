CC=gcc -Wall

PROGS=jogador server mestre

all: $(PROGS)

clean:
	rm -f $(PROGS)

server:	server.c
	$(CC) server.c -o server -lrt

jogador:	jogador.c
	$(CC) jogador.c -o jogador -lrt

mestre:	mestre.c
	$(CC) mestre.c -o mestre -lrt	
	
