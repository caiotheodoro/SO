#-----------------------------------------------------------------------
# Programas.
#-----------------------------------------------------------------------
PROGRAMS = \
    ex02_signal_alarm \
    ex01_simple_signal_handler \
    ex03_signal_raise \
    ex04_signal_mask \

#-----------------------------------------------------------------------
# Bibliotecas.
#-----------------------------------------------------------------------
LIBRARIES =

#-----------------------------------------------------------------------
all: $(PROGRAMS) fim

#-----------------------------------------------------------------------
# Compilação e geração dos executáveis.
#-----------------------------------------------------------------------
ex02_signal_alarm: ex02_signal_alarm.o
	$(CC) $(CFLAGS) -o ex02_signal_alarm ex02_signal_alarm.o $(LIBRARIES)
	rm ex02_signal_alarm.o

ex01_simple_signal_handler: ex01_simple_signal_handler.o
	$(CC) $(CFLAGS) -o ex01_simple_signal_handler ex01_simple_signal_handler.o $(LIBRARIES)
	rm ex01_simple_signal_handler.o

ex03_signal_raise: ex03_signal_raise.o
	$(CC) $(CFLAGS) -o ex03_signal_raise ex03_signal_raise.o $(LIBRARIES)
	rm ex03_signal_raise.o

ex04_signal_mask: ex04_signal_mask.o
	$(CC) $(CFLAGS) -o ex04_signal_mask ex04_signal_mask.o $(LIBRARIES)
	rm ex04_signal_mask.o
	
#-----------------------------------------------------------------------
# Objetos.
#-----------------------------------------------------------------------
ex02_signal_alarm.o: ex02_signal_alarm.c
	$(CC) $(CFLAGS) -c ex02_signal_alarm.c

ex01_simple_signal_handler.o: ex01_simple_signal_handler.c
	$(CC) $(CFLAGS) -c ex01_simple_signal_handler.c

ex03_signal_raise.o: ex03_signal_raise.c
	$(CC) $(CFLAGS) -c ex03_signal_raise.c

ex04_signal_mask.o: ex04_signal_mask.c
	$(CC) $(CFLAGS) -c ex04_signal_mask.c

clean: 
	rm ex02_signal_alarm ex01_simple_signal_handler ex03_signal_raise ex04_signal_mask

fim:
	@echo "Finished."
