#include "queue.h"

int buf_size;
static int front;
static int back;
struct Req** buffer = NULL;
int nItems;
// Initialise a buffer of requests to a certain size s
Req** init(int s)
{
    if(buffer == NULL)
    {
        buf_size = s;
        buffer = (Req**)calloc(buf_size, sizeof(Req*));
        front = 0;
        back = 0;
        nItems = 0;
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
    if(buffer)
    {
        if(!isFull() && new_req != NULL)
        {
            buffer[back%buf_size] = new_req;
            back++;
            nItems++;
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
    } else printf("[ERROR] Buffer doesn't, cannot add.\n");
}

/* get
* returns a request to the caller, and sets the request to NULL in the buffer
* moves the front to incremented 1 place.
*/
Req* get(void)
{
    Req* out = NULL;
    if (buffer && !isEmpty())
    {
        out = buffer[front%buf_size];
        buffer[front%buf_size] = NULL;
        // one item consideration
        front++;
        nItems--;
    }
    return out;
}

int isFull()
{
    int res = 0;
    if(buffer){
        res = nItems == buf_size-1;
        // if (front != 0)
        // {
        //     res = (back % front) == back && buffer[back%buf_size] != NULL;
        // }
        // else // avoid div by 0
        // {
        //     res = back == buf_size && buffer[back%buf_size] != NULL; // wrap around consideration
        // }
    }
    return res; //will always result as 1 for full queue
}

int isEmpty()
{
    int out = 0;
    if(buffer){
        out = nItems == 0;
        // out = front == back && buffer[front%buf_size] == NULL;
    }
    return out;
}

void destroy()
{
    free(buffer);
}