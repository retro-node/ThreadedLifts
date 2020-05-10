#ifndef FORKED_LIFTS_H
#define FORKED_LIFTS_H 
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include "FileManager.h"
#include "queue.h"
#ifndef PTHREADED_LIFTS_H
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
#endif
void *lift_r(void*);
void *lift(void*);

#endif
