#ifndef PTHREAD_LIFTS_H
#define PTHREAD_LIFTS_H 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Req {
    int source;
    int dest;
    int req_no;
} Req;

typedef struct lift_move {
    int lift_no;
    int lift_origin;
    Req* request;
    int num_moves;
} lift_move; 

void lift_r(void);
void lift(int);
#include "FileManager.h"
#endif
