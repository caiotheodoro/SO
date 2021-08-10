CC=gcc -Wall

PROGS=prod_cons

all: $(PROGS)

clean:
	rm -f $(PROGS)

prod_cons: main.c resource_monitor.c resource_monitor.h
	$(CC) main.c resource_monitor.c -o prod_cons -lpthread
