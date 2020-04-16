/*************************
* LIFT REQUESTS USING PTHREADS
* Responsible for request buffer.
* Author: Jeff Dugmore
* Date Created: 09/04/2020
* Date Modified: 13/04/2020
************************/

#include "pthread_lifts.h"


pthread_mutex_t lock;

pthread_t ptid[NUM_THREADS];

struct lift_params { // used only to allow multi param pthread create
    int lift_no;
    int lift_time;
};
/******************
* lift_r
* Calls request to get single set of instructions and put it into the buffer.
* Specification: The function puts a request to the buffer when there is available
space. Otherwise, Lift-R is blocked waiting for available space in the buffer.
*****************/
void *lift_r(void* number)
{
    int m = *((int*)number);
    Req* req = (Req*)request();
    Req** buffer = (Req**)calloc(m, sizeof(Req*));
    if(req == NULL && req->req_no <= 0) printf("Invalid request tried to load into buffer.");
    else 
    {
        buffer[req->req_no-1] = req;
        write_request(req);
    }
    return NULL;
}

/******************
* lift
* Gets request from the buffer and adds imformation required by lift_move struct
* Specification: Function takes a req from buffer when buffer is not locked, otherwise
is blocked waiting to take next request.
*****************/
void *lift(void* args)
{
    struct lift_params * params = (struct lift_params*)args;
    Req* request = NULL; //fetch from buffer
    const int this_lift_no = params->lift_no;
    static int lift_origin = 0;
    lift_move* movement = (lift_move*)calloc(1, sizeof(lift_move));
    movement-> lift_no = this_lift_no;
    movement-> lift_origin = lift_origin;
    movement-> request = request;
    movement-> num_moves = 0;
    sleep(params->lift_time);
    free(params);
    pthread_mutex_lock(&lock);
    write_completed(movement); // will free movement and request
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char const *argv[])
{
    if( argc != 3 ) printf("Usage:\n"
    "\tlift_sim_A m t\n"
    "\t\tm = Size of buffer to be used by lifts\n"
    "\t\tt = time taked to travel for all lifts\n");
    else
    {
        int m = atoi(argv[1]);
        int t = atoi(argv[2]);
        printf("[INFO] Initializing simulation using buffer size %d elements and lift time of %d seconds\n", m, t);
        // initialize mutex
        if(pthread_mutex_init(&lock, NULL) != 0 ) printf("[ERROR] Initialization failed.\n");
        else
        {
            if(pthread_create(&ptid[0],NULL, lift_r, &m) != 0) printf("[ERROR] Creating Lift_R failed: Terminating\n"); // create lift_r
            else
            {
                for(int i = 1; i < NUM_THREADS-1; i++) // i from 1 to 3 to make lift 1,2 & 3
                {
                    struct lift_params* params = (struct lift_params*)calloc(1, sizeof(struct lift_params));
                    params->lift_no=i;
                    params->lift_time=t;
                    if(pthread_create(&(ptid[i]), NULL, lift, (void*)params) != 0) printf("[ERROR] Creating lift %d failed.\n", i); // TODO possible probable cause of issue thread names
                }
                for( int ii = 0; ii < NUM_THREADS-1; ii++) 
                {
                    pthread_join(ptid[ii], NULL);
                }
            }
            pthread_mutex_destroy(&lock);
        }
    }
    return 0;
}
