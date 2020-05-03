#include "queue.h"
struct Req* buffer = NULL;
// Initialise a buffer of requests to a certain size s
Req* init(int s)
{
    if(buffer == NULL)
    {
        buffer = (Req*)calloc(s, sizeof(Req));
        place = 0;
    }
    else
    {
        #ifdef DEBUG
        printf("[ERROR] Buffer already initialised!\n");
        #endif
    }
    return buffer;
    
}
/* add
* Add a request to the buffer, and increment the place value
*/
void add(Req* new_req)
{
    if(place == buf_size-1)// || buffer[buf_size-1] != NULL)
    {
        #ifdef DEBUG
        printf("[ERROR] Buffer is full!\n");
        #endif
    }
    else
    {
        buffer[place] = *new_req;
        place++;
    }
}

/* get
* returns a request to the caller, and sets the request to NULL in the buffer
* moves the place to new actual length of queue
*/
Req* get(void)
{
    Req* out;
    if(place == 0)
    {
        out = &buffer[0];
        #ifdef DEBUG
        printf("[ERROR] Buffer is empty\n");
        #endif
    }
    else 
    {
        out = &buffer[place];
        // buffer[place] = NULL;
        place--;
    }
    return out;
}
int isFull(void)
{
    return place == buf_size-1;
}