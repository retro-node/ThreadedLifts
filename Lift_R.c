/*************************
* LIFT REQUESTS
* Responsible for request buffer.
* Author: Jeff Dugmore
* Date Created: 05/04/2020
* Date Modified: 05/04/2020
************************/

#include "Lift_R.h"
#include <stdio.h>
#include <stdlib.h>

/******************
* lift_r
* Calls request to get single set of instructions and put it into the buffer.
* Specification: The function puts a request to the buffer when there is available
space. Otherwise, Lift-R is blocked waiting for available space in the buffer.
*****************/
void lift_r(void)
{
    static int line=0;
    Instructions* req = request(line);
    Instructions** all_instructions = (Instructions**)calloc(1, sizeof(Instructions*));
    write_request(req->start, req->end); //TODO requires mutex
    line++;
}
