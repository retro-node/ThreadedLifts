#ifndef PTHREAD_LIFTS_H
#define PTHREAD_LIFTS_H 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "FileManager.h"
#include "queue.h"

typedef struct Req {
    int source;
    int dest;
    int req_no;
} Req;

typedef struct lift_move {
    int lift_no;
    int lift_origin;
    struct Req* request;
    int num_moves;
} lift_move; 

#define NUM_THREADS 4 //total number of threads 3x lift & 1x lift_r
void *lift_r(void*);
void *lift(void*);

#endif
