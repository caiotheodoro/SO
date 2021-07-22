CC=gcc -Wall
CCW=gcc -w

PROGS=ex01 ex02 size_pipe

all: $(PROGS)

clean:
	rm -f $(PROGS)

ex01:	ex01_simple_pipe.c
	$(CC) ex01_simple_pipe.c -o ex01

ex02:	ex02_write_read_strings_pipe.c
	$(CC) ex02_write_read_strings_pipe.c -o ex02

size_pipe: size_pipe.c
	$(CCW) size_pipe.c -o size_pipe
