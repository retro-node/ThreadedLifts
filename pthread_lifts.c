/*************************
* LIFT REQUESTS USING PTHREADS
* Responsible for request buffer.
* Author: Jeff Dugmore
* Date Created: 09/04/2020
* Date Modified: 13/04/2020
************************/

#include "pthread_lifts.h"

pthread_mutex_t fileio_lock;
pthread_mutex_t buffer_lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


struct lift_params { // used only to allow multi param pthread create
    int lift_no;
    int lift_time;
};

// size_t m = 0;
const int* LIFT_TIME;
int cond_ret;
int static lift_origin[3]; // determine how to make static
// lift_origin[0] = 1;
// lift_origin[1] = 1;
// lift_origin[2] = 1;
Req** req_buffer;
/******************
* lift_r
* Calls request to get single set of instructions and put it into the buffer.
* Specification: The function puts a request to the buffer when there is available
space. Otherwise, Lift-R is blocked waiting for available space in the buffer.
*****************/
void *lift_r(void* number)
{
    #ifdef DEBUG
    printf("[DEBUG] This is Lift R responding\n");
    #endif
    size_t m = *(int*)number;
    pthread_mutex_lock(&buffer_lock);
    req_buffer = init(m);
    Req* req = (Req*)request();
    pthread_mutex_unlock(&buffer_lock);
    do
    {
        pthread_mutex_lock(&buffer_lock);
        // needs a mutex as other operations manipulate front and back queue counters
        while(req != NULL && !isFull())
        {
            // queue manipulation requires mutex
            add(req);
            req = (Req*)request();
            pthread_cond_signal(&cond); // tell other threads to come eat        
            #ifdef QUEUEV
            pthread_mutex_lock(&fileio_lock);
            if (req_buffer[0]) printf("| %d : %d |", 0, req_buffer[0]->req_no);
            else printf("| 0 : EMPTY |");
            for(int i = 1; i < (int)m; i++)
            {
                if(req_buffer[i])
                {
                    printf(" %d : %d |", i, req_buffer[i]->req_no);
                }else{
                    printf(" %d : %s |", i, "EMPTY");
                }
            }
            printf("\n");
            pthread_mutex_unlock(&fileio_lock);
            #endif
            
        }
        pthread_mutex_unlock(&buffer_lock);
    }while(req != NULL);
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
    // int * lift_no = (int*)args;
    struct lift_params params = *(struct lift_params*)args;

    // Req* request = NULL; //fetch from buffer
    const int this_lift_no = params.lift_no;

    #ifdef DEBUG
    pthread_mutex_lock(&fileio_lock);
    printf("[DEBUG] This is lift %d responding with %d lift time\n", this_lift_no, params.lift_time);
    pthread_mutex_unlock(&fileio_lock);
    #endif
    lift_move* movement = (lift_move*)calloc(1, sizeof(lift_move));
    movement-> lift_no = this_lift_no;
    movement-> lift_origin = lift_origin[this_lift_no-1];
    pthread_mutex_lock(&buffer_lock);
    while(isEmpty())
    {
        pthread_cond_wait(&cond, &buffer_lock);
    }

    Req* new_req = get();
    pthread_mutex_unlock(&buffer_lock);
    movement-> request = new_req;
    movement-> num_moves = 0;
    int destination = movement->request->dest;
    sleep(params.lift_time);
    pthread_mutex_lock(&fileio_lock);
    write_completed(movement); // will free movement and movement->request
    pthread_mutex_unlock(&fileio_lock);
    lift_origin[this_lift_no-1] = destination;
    #ifdef DEBUG
    pthread_mutex_lock(&fileio_lock);
    printf("[DEBUG] Lift %d is now at %d\n", this_lift_no, lift_origin[this_lift_no-1]);
    printf("[DEBUG] This is lift %d completing\n", this_lift_no);
    pthread_mutex_unlock(&fileio_lock);
    #endif
    return NULL;
}

int main(int argc, char const *argv[])
{
    if( argc != 3) printf("Usage:\n"
                            "\tlift_sim_A m t\n"
                            "\t\tm = Size of buffer to be used by lifts\n"
                            "\t\tt = time taken to travel for all lifts\n");
    else
    {
        pthread_t ptid[NUM_THREADS];
        int m = atoi(argv[1]);
        const int t = atoi(argv[2]);
        LIFT_TIME=&t;
        if (t >= 0 && m >=0 )
        {

            printf("[INFO] Initializing simulation using buffer size %d elements and lift time of %d seconds\n", m, t);
            // initialize mutex
            if(pthread_mutex_init(&buffer_lock, NULL) != 0 && pthread_mutex_init(&fileio_lock, NULL) != 0) printf("[ERROR] Initialization failed.\n");
            else
            {
                int liftr_status = pthread_create(&ptid[0], NULL, lift_r, &m);
                if( liftr_status != 0) printf("[ERROR] Creating Lift_R failed: Terminating\n"); // create lift_r
                else
                {
                    printf("[INFO] Successfully created lift R\n");
                    for(int i = 1; i < NUM_THREADS; i++) // i from 1 to 3 to create lift 1,2 & 3
                    {
                        struct lift_params* params = (struct lift_params*)calloc(1, sizeof(struct lift_params));
                        params->lift_no=i;
                        params->lift_time=t;
                        int status = pthread_create(&(ptid[i]), NULL, lift, params); // TODO possible probable cause of issue thread names
                        if (status == 0) printf("[INFO] Successfully created lift %d\n", i);
                        else printf("[ERROR] Creating lift %d failed.\n", i); 
                    }
                    for( int ii = 0; ii < NUM_THREADS; ii++) 
                    {
                        int thread_res = pthread_join(ptid[ii], NULL);
                        #ifdef DEBUG
                        if (!thread_res) {
                            if( ii != 0 ) printf("[DEBUG] Joined Thread %d\n", ii);
                            else printf("[DEBUG] Joined Thread R\n");
                        }
                        else {
                            if( ii != 0 ) printf("[ERROR] Failed to join Thread %d\n", ii);
                            else printf("[ERROR] Failed to join Thread R\n");
                        }
                        #endif
                    }
                    // #ifdef DEBUG
                    // printf("[DEBUG] Joining Thread R\n");
                    // #endif
                    // pthread_join(ptid[0], NULL);
                }
                free(req_buffer);
                pthread_mutex_destroy(&fileio_lock);
                pthread_mutex_destroy(&buffer_lock);
            }
        }
        else
        {
            printf("Usage:\n"
                            "\tlift_sim_A m t\n"
                            "\t\tm = Size of buffer to be used by lifts\n"
                            "\t\tt = time taken to travel for all lifts\n"
                    "!Caution! Buffer size and time need to be positive integers!\n");

        }
    }
    pthread_cond_destroy(&cond);
    return 0;
}
