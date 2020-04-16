#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include "pthread_lifts.h"

#define FILE_IN "sim_in"
#define FILE_OUT "sim_out"


void* request(void);
int write_request(void*);
int write_completed(void*);

#endif
