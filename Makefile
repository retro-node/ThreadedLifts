CFLAGS = -Wall -Wextra -std=c18 
CC = gcc
OBJ = FileManager.o pthread_lifts.o testing.o #forked_lifts.o
EXECS =  lift_sim_A tests #lift_sim_B
ifdef DEBUG
CFLAGS += -DDEBUG
SECRET : clean $(EXEC)
endif
# ifdef UNIT_TEST
# CFLAGS += -DUNIT_TEST
# SECRET : clean $(EXEC)
# endif

# $(EXEC) : $(OBJ)
# 	$(CC) $(OBJ) -o $(EXEC)
all : $(EXECS)

lift_sim_A : pthread_lifts.o FileManager.o
	$(CC) pthread_lifts.o FileManager.o -o lift_sim_A

# lift_sim_B : forked_lifts.o
# 	$(CC) forked_lifts.o -o lift_sim_B

tests : testing.o FileManager.o 
	$(CC) testing.o FileManager.o -o tests

FileManager.o : FileManager.c FileManager.h pthread_lifts.h
	$(CC) $(CFLAGS) -g FileManager.c -c

# forked_lifts.o : forked_lifts.c forked_lifts.h FileManager.h
# 	$(CC) $(CFLAGS) -g forked_lifts.c -c

pthread_lifts.o : pthread_lifts.c pthread_lifts.h FileManager.h 
	$(CC) $(CFLAGS) -lpthread -g pthread_lifts.c -c 

testing.o : testing.c testing.h FileManager.h pthread_lifts.h
	$(CC) $(CFLAGS) -g testing.c -c

clean:
	rm -f $(EXECS) $(OBJ) sim_out
