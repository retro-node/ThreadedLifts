/*************************
* LIFT REQUESTS
* Responsible for request buffer.
* Author: Jeff Dugmore
* Date Created: 09/04/2020
* Date Modified: 09/04/2020
************************/

#include "Lifts.h"

/******************
* lift_r
* Calls request to get single set of instructions and put it into the buffer.
* Specification: The function puts a request to the buffer when there is available
space. Otherwise, Lift-R is blocked waiting for available space in the buffer.
*****************/
void lift_r(void)
{
    Req* req = request();
    Req** all_requests = (Req**)calloc(1, sizeof(Req*)); // BUFFER
    write_request(req); //TODO requires mutex
}
