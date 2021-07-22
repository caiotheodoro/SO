CC=gcc -Wall

PROGS=chat_cliente chat_servidor

all: $(PROGS)

clean:
	rm -f $(PROGS)

chat_cliente:	chat_cliente.c
	$(CC) chat_cliente.c -o chat_cliente

chat_servidor:	chat_servidor.c
	$(CC) chat_servidor.c -o chat_servidor
