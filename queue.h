#ifndef QUEUE_H
#define QUEUE_H
#include "pthread_lifts.h"


int buf_size;
static int place;

struct Req* init(int);
void add(struct Req*);
struct Req* get(void);
int isFull(void);

#endif