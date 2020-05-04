#ifndef QUEUE_H
#define QUEUE_H
#include "pthread_lifts.h"

struct Req** init(int);
void add(struct Req*);
struct Req* get(void);
int isFull(void);
int isEmpty(void);
void balance(void);

#endif