CC=gcc -Wall

P1=ex00
P2=ex01
P3=ex02
P4=ex03
P5=ex04

FLAGS=-lpthread

PROGS=$(P1) $(P2) $(P3) $(P4) $(P5)

all: $(PROGS)

$(P1):	$(P1).c
	$(CC) $(P1).c -o $(P1) $(FLAGS)
	
$(P2):	$(P2).c
	$(CC) $(P2).c -o $(P2) $(FLAGS)
	
$(P3):	$(P3).c
	$(CC) $(P3).c -o $(P3) $(FLAGS)
	
$(P4):	$(P4).c
	$(CC) $(P4).c -o $(P4) $(FLAGS)
	
$(P5):	$(P5).c
	$(CC) $(P5).c -o $(P5) $(FLAGS)
	
clean:
	rm $(PROGS)
