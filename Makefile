CFLAGS = -Wall -Wextra -std=c18
CC = gcc
OBJ = FileManager.o
HDRS = FileManager.h
EXEC = FileManagerTest

ifdef DEBUG
CFLAGS += -DDEBUG
SECRET : clean $(EXEC)
endif

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

FileManager.o : FileManager.c FileManager.h
	$(CC) $(CFLAGS) -g FileManager.c -c
    
clean:
	rm -f $(EXEC) $(OBJ)
