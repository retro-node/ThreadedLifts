#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include "Lifts.h"

#define FILE_IN "sim_in"
#define FILE_OUT "sim_out"


Req* request(void);
int write_request(Req*);
int write_completed(lift_move*, int);

#endif
