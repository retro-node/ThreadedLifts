#ifndef FILEMANAGERFORKED_H
#define FILEMANAGERFORKED_H
#include <stdio.h>
#include <stdlib.h>
#include "forked_lifts.h"
#ifndef FILEMANAGER_H
#define FILE_IN "sim_in"
#define FILE_OUT "sim_out"

void init_mem_fork(void);
void* request(void);
int write_request(void*);
int write_completed(void*);
#endif
#endif
