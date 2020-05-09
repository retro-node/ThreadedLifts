CFLAGS = -Wall -Wextra -std=c18 
CC = gcc
OBJ = FileManager.o pthread_lifts.o testing.o queue.o #forked_lifts.o
EXECS = lift_sim_A tests #lift_sim_B

ifdef DEBUG
CFLAGS += -DDEBUG
SECRET : clean $(EXECS)
endif

ifdef QUEUEV
CFLAGS += -DQUEUEV
SECRET : clean $(EXECS)
endif

# ifdef UNIT_TEST
# CFLAGS += -DUNIT_TEST
# SECRET : clean $(EXEC)
# endif

all : $(EXECS)

tests : testing.o FileManager.o
	$(CC) testing.o FileManager.o -o tests

lift_sim_A : pthread_lifts.o FileManager.o queue.o
	$(CC) pthread_lifts.o FileManager.o queue.o -o lift_sim_A -lpthread

pthread_lifts.o : pthread_lifts.c FileManager.h pthread_lifts.h queue.h
	$(CC) $(CFLAGS) -g pthread_lifts.c -c

queue.o : queue.c queue.h pthread_lifts.h
	$(CC) $(CFLAGS) -g queue.c -c

# lift_sim_B : forked_lifts.o
# 	$(CC) forked_lifts.o -o lift_sim_B

# forked_lifts.o : forked_lifts.c forked_lifts.h FileManager.h
# 	$(CC) $(CFLAGS) -g forked_lifts.c -c

FileManager.o : FileManager.c FileManager.h pthread_lifts.h
	$(CC) $(CFLAGS) -g FileManager.c -c

testing.o : testing.c testing.h FileManager.h pthread_lifts.h
	$(CC) $(CFLAGS) -g testing.c -c

clean:
	rm -f $(EXECS) $(OBJ)
