#ifndef QUEUE_H
#define QUEUE_H
#include "pthread_lifts.h"
#define NO_BALANCE 0
#define BALANCE 1
struct Req** init(int);
void add(struct Req*);
struct Req* get(void);
int isFull(int);
int isEmpty(int);

#endif