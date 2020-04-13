/*************************
* LIFT REQUESTS USING PTHREADS
* Responsible for request buffer.
* Author: Jeff Dugmore
* Date Created: 09/04/2020
* Date Modified: 13/04/2020
************************/

#include "pthread_lifts.h"
pthread_mutex_t lock;
/******************
* lift_r
* Calls request to get single set of instructions and put it into the buffer.
* Specification: The function puts a request to the buffer when there is available
space. Otherwise, Lift-R is blocked waiting for available space in the buffer.
*****************/
void lift_r(void)
{
    Req* req = request();
    Req* buffer[50];
    write_request(req); //TODO requires mutex
}

/******************
* lift
* Gets request from the buffer and adds imformation required by lift_move struct
* Specification: Function takes a req from buffer when buffer is not locked, otherwise
is blocked waiting to take next request.
*****************/
void lift(int lift_no)
{
    Req* request;
    const int this_lift_no = lift_no;
    static int lift_origin = 0;
    lift_move* movement = (lift_move*)calloc(1, sizeof(lift_move));
    movement-> lift_no = this_lift_no;
    movement-> lift_origin = lift_origin;
    movement-> request = request;
    movement-> num_moves = 0;
    //aquire mutex
    write_completed(movement); // will free movement and request
    //release mutex
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
