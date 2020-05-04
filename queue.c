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
        buffer = (Req**)calloc(s, sizeof(Req*));
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
    if(!isFull())
    {
        buffer[back] = new_req;
        back++;
    }
    else
    {
        #ifdef DEBUG
        printf("[ERROR] Buffer is full, cannot add.");
        #endif
    }
    
}

/* get
* returns a request to the caller, and sets the request to NULL in the buffer
* moves the front to incremented 1 place.
*/
Req* get(void)
{
    balance();
    Req* out = buffer[front];
    buffer[front] = NULL;
    if( front != back ) // avoid false isFull
    {
        front++;
    }
    return out;
}

int isFull(void)
{
    balance();
    int res = 0;
    if (front != 0)
    {
        res = (back % front) == back;
    }
    else // avoid div by 0
    {
        res = back == buf_size-1;
    }
    

    return res; //will always result as 1 for full queue
}

int isEmpty(void)
{
    return front == back && buffer[front] == NULL;
}

// method to enforce wrapped around front and back values
void balance()
{
    if (front >= buf_size) front = front % (buf_size-1); // keep within buffer range
    if (back >= buf_size) back = back % (buf_size-1);
}