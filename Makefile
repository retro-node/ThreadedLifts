CFLAGS = -Wall -Wextra -std=c18
CC = gcc
OBJ = main.o FileManager.o Lift_R.o
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

Lift_R.o : Lift_R.c Lift_R.h
	$(CC) $(CFLAGS) -g Lift_R.c -c
    
FileManager.o : FileManager.c FileManager.h
	$(CC) $(CFLAGS) -g FileManager.c -c
    
main.o : main.c $(HDRS)
	$(CC) $(CFLAGS) -g main.c -c
    
clean:
	rm -f $(EXEC) $(OBJ)
