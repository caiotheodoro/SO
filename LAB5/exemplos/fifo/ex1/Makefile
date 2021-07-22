CC=gcc -Wall

PROGS=read_fifo write_fifo

all: $(PROGS)

clean:
	rm -f $(PROGS)

read_fifo:	read_fifo.c
	$(CC) read_fifo.c -o read_fifo

write_fifo:	write_fifo.c
	$(CC) write_fifo.c -o write_fifo
