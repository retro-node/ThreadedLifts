CFLAGS = -Wall -Wextra -std=c18
CC = gcc
OBJ = main.o FileManager.o
HDRS = main.h FileManager.h
EXEC = ThreadedLifts

ifdef DEBUG
CFLAGS += -DDEBUG
SECRET : clean $(EXEC)
endif

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

FileManager.o : FileManager.c FileManager.h
	$(CC) $(CFLAGS) -g FileManager.c -c
    
main.o : main.c $(HDRS)
	$(CC) $(CFLAGS) -g main.c -c
    
clean:
	rm -f $(EXEC) $(OBJ)
