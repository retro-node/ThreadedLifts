#include "queue.h"

int buf_size;
static int front;
static int back;
struct Req** buffer = NULL;

// Initialise a buffer of requests to a certain size s
Req** init(int s)
{
    if(buffer == NULL)
    {
        buf_size = s;
        buffer = (Req**)calloc(buf_size, sizeof(Req*));
        front = 0;
        back = 0;
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
* Add a request to the buffer, and increment the back value
*/
void add(Req* new_req)
{
    if(!isFull() && new_req != NULL)
    {
        buffer[back] = new_req;
        back++;
    }
    #ifdef DEBUG
    else if (new_req == NULL) 
    {
        printf("[ERROR] Null request cannot be added.\n");
    }
    else
    {
        printf("[ERROR] Buffer is full, cannot add.\n");
    }
    #endif
}

/* get
* returns a request to the caller, and sets the request to NULL in the buffer
* moves the front to incremented 1 place.
*/
Req* get(void)
{
    Req* out = NULL;
    if (!isEmpty())
    {
        out = buffer[front];
        buffer[front] = NULL;
        // one item consideration
        if( front != back ) front++; // dissallow front to falsely preceed back
    }
    return out;
}

int isFull(void)
{
    if (front >= buf_size) front = (front % (buf_size-1)) ; //avoid func overhead for use in while loop
    if (back-1 >= buf_size) back = (back % (buf_size-1)) ;
    int res = 0;
    if (front != 0)
    {
        res = (back % front) == back;
    }
    else // avoid div by 0
    {
        res = back == buf_size; // wrap around consideration
    }
    
    return res; //will always result as 1 for full queue
}

int isEmpty(void)
{
    if (front >= buf_size) front = (front % (buf_size-1)) ; // keep within buffer range
    if (back >= buf_size) back = (back % (buf_size-1)) ;
    return front == back && buffer[front] == NULL;
}


// TODO destroy func