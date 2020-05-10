/*************************
* LIFT REQUESTS USING FORKS
* Responsible for sharing memory.
* Author: Jeff Dugmore
* Date Created: 10/05/2020
* Date Modified: 11/05/2020
************************/

#include "forked_lifts.h"

sem_t fileio_lock;
sem_t buffer_lock;

struct lift_params { // used only to allow multi param sem create
    int lift_no;
    int lift_time;
};

// size_t m = 0;
const int* LIFT_TIME;
static int lift_origin[3] = {1, 1, 1}; // TODO move inside child
Req** req_buffer;
int finished = 0;
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
    sem_wait(&buffer_lock);
    req_buffer = init(m);
    Req* req = (Req*)request();
    sem_wait(&fileio_lock);
    if (req != NULL) write_request(req);
    sem_post(&fileio_lock);
    sem_post(&buffer_lock);
    do
    {
        sem_wait(&buffer_lock);
        // needs a mutex as other operations manipulate front and back queue counters
        if(req != NULL && !isFull())
        {
            // queue manipulation requires mutex
            add(req);
            req = (Req*)request();
            sem_wait(&fileio_lock);
            if (req != NULL) write_request(req);
            sem_post(&fileio_lock);
            #ifdef QUEUEV
            sem_wait(&fileio_lock);
            for (int ii = 0 ; ii < (int)m; ii++) printf("--------");
            printf("\n");
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
            sem_post(&fileio_lock);
            #endif
            
        }
        sem_post(&buffer_lock);
    }while(req != NULL);
    // sem_wait(&buffer_lock);
    // while(!isEmpty) sem_cond_signal(&cond);
    // sem_post(&buffer_lock);
    finished = 1;
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
    sem_wait(&fileio_lock);
    printf("[DEBUG] This is lift %d responding with %d lift time\n", this_lift_no, params.lift_time);
    sem_post(&fileio_lock);
    #endif
    while(!finished)
    {
        lift_move* movement = (lift_move*)calloc(1, sizeof(lift_move));
        movement-> lift_no = this_lift_no;
        movement-> lift_origin = lift_origin[this_lift_no-1];
        sem_wait(&buffer_lock);
        Req* new_req = get();
        sem_post(&buffer_lock);
        #ifdef DEBUG
        sem_wait(&fileio_lock);
        printf("[DEBUG] Lift %d moving from %d to %d\n", this_lift_no, new_req->source, new_req->dest);
        sem_post(&fileio_lock);
        #endif
        movement-> request = new_req;
        movement-> num_moves = 0;
        int destination = movement->request->dest;
        sleep(params.lift_time);
        sem_wait(&fileio_lock);
        write_completed(movement); // will free movement and movement->request
        sem_post(&fileio_lock);
        lift_origin[this_lift_no-1] = destination;
        #ifdef DEBUG
        sem_wait(&fileio_lock);
        printf("[DEBUG] Lift %d is now at %d\n", this_lift_no, lift_origin[this_lift_no-1]);
        printf("[DEBUG] This is lift %d completing\n", this_lift_no);
        sem_post(&fileio_lock);
        #endif
    }
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
        pid_t ptid[NUM_THREADS];
        int m = atoi(argv[1]);
        const int t = atoi(argv[2]);
        LIFT_TIME=&t;
        if (t >= 0 && m >=0 )
        {

            printf("[INFO] Initializing simulation using buffer size %d elements and lift time of %d seconds\n", m, t);
            // initialize mutex
            if(sem_init(&buffer_lock, 0, m) != 0 && sem_init(&fileio_lock, 0, 1) != 0) printf("[ERROR] Initialization failed.\n");
            else
            {
                // initialize shared memory for buffer access

                ptid[0] = fork();
                if (ptid[0] == 0) lift_r((void*)&m);
                else if( ptid[0] < 0 ) printf("[ERROR] Creating Lift_R failed: Terminating\n"); // create lift_r
                else
                {
                    printf("[INFO] Successfully created lift R\n");
                    for(int i = 1; i < NUM_THREADS; i++) // i from 1 to 3 to create lift 1,2 & 3
                    {
                        struct lift_params* params = (struct lift_params*)calloc(1, sizeof(struct lift_params));
                        params->lift_no=i;
                        params->lift_time=t;
                        ptid[i] = fork();
                        if (ptid[i] > 0) lift((void*)params); // TODO memory
                        else if (ptid[i] == 0) printf("[INFO] Successfully created lift %d\n", i);
                        else printf("[ERROR] Creating lift %d failed.\n", i); 
                    }
                    for( int ii = 0; ii < NUM_THREADS; ii++) 
                    {
                        // TODO join children
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
                    // sem_join(ptid[0], NULL);
                }
                free(req_buffer);
                sem_destroy(&fileio_lock);
                sem_destroy(&buffer_lock);
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
    return 0;
}
