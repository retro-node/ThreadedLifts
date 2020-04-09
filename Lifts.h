#ifndef LIFTS_H
#define LIFTS_H 
#include <stdio.h>
#include <stdlib.h>

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
#include "FileManager.h"
#endif
