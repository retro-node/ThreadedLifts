#ifndef QUEUE_H
#define QUEUE_H
#include "pthread_lifts.h"
// #define  0
// #define  1
struct Req** init(int);
void add(struct Req*);
struct Req* get(void);
int isFull(void); //here
int isEmpty(void); //here
int exists(void);
void destroy(void);

#endif