CFLAGS = -Wall -Wextra -std=c18
CC = gcc
OBJ = main.o FileManager.o Lifts.o
HDRS = main.h FileManager.h
EXEC = ThreadedLifts

ifdef DEBUG
CFLAGS += -DDEBUG
SECRET : clean $(EXEC)
endif
ifdef UNIT_TEST
CFLAGS += -DUNIT_TEST
SECRET : clean $(EXEC)
endif

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

Lifts.o : Lifts.c Lifts.h
	$(CC) $(CFLAGS) -g Lifts.c -c
    
FileManager.o : FileManager.c FileManager.h
	$(CC) $(CFLAGS) -g FileManager.c -c
    
main.o : main.c $(HDRS)
	$(CC) $(CFLAGS) -g main.c -c
    
clean:
	rm -f $(EXEC) $(OBJ) sim_out
