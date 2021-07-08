CC=gcc
CFLAGS=-I.
DEPS = matriz.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matriz: matriz.o test_matriz.o 
	$(CC) -o test_matriz matriz.o test_matriz.o

clean: 
	rm *.o test_matriz
