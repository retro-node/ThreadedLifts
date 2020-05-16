/*************************
* LIFT REQUESTS USING FORKS
* Responsible for sharing memory.
* Author: Jeff Dugmore
* Date Created: 10/05/2020
* Date Modified: 11/05/2020
************************/

#include "forked_lifts.h"

sem_t *fileio_lock;
sem_t *buffer_lock;
sem_t *cond;

struct lift_params { // used only to allow multi param sem create
    int lift_no;
    int lift_time;
};

// struct child_data{
//     struct lift_params* params; // lift specific
//     Req** req_buffer;
//     sem_t* fileio_lock;
//     sem_t* buffer_lock;
//     sem_t* cond;
//     int running;
// };
int *running;
static int lift_origin[3] = {1,1,1}; 
Req** req_buffer;

// /* Unused method for reference
// * Woulda been nice
// * from https://www.cs.uaf.edu/2012/spring/cs641/lecture/04_05_fork_mmap.html accessed 16/05/2020
// */
// void* shared_malloc(int len)
// {
//     void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
//     return ptr;
// }


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
    sem_wait(buffer_lock);
    req_buffer = init(m);
    Req* req = (Req*)request();
    sem_wait(fileio_lock);
    if (req != NULL) write_request(req);    
    sem_post(fileio_lock);
    sem_post(buffer_lock);
    do
    {
        if(!isFull(NO_BALANCE)) // release mutex when full
        {
            sem_wait(buffer_lock);
            while(isFull(BALANCE))
            {
                sem_wait(buffer_lock);
            }
            // queue manipulation requires mutex
            add(req);
            req = (Req*)request();
            sem_wait(fileio_lock);
            if (req != NULL) write_request(req);
            sem_post(fileio_lock);   
            #ifdef QUEUEV
            sem_wait(fileio_lock);
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
            sem_post(fileio_lock);
            #endif
            sem_post(buffer_lock);
        }
    }while(req != NULL);
    *running = 0;
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
    sem_wait(fileio_lock);
    printf("[DEBUG] This is lift %d responding with %d lift time\n", this_lift_no, params.lift_time);
    sem_post(fileio_lock);
    #endif
    while(*running)
    {
        while(!isEmpty(NO_BALANCE)){
            lift_move* movement = (lift_move*)calloc(1, sizeof(lift_move));
            movement-> lift_no = this_lift_no;
            movement-> lift_origin = lift_origin[this_lift_no-1];
            sem_wait(buffer_lock);
            Req* new_req = get();
            sem_post(cond);
            if (new_req != NULL)
            {
                #ifdef DEBUG
                sem_wait(fileio_lock);
                printf("[DEBUG] Lift %d going from %d to %d\n", this_lift_no, new_req->source, new_req->dest);
                sem_post(fileio_lock);
                #endif
                movement-> request = new_req;
                movement-> num_moves = 0;
                int destination = movement->request->dest;
                sleep(params.lift_time);
                sem_wait(fileio_lock);
                write_completed(movement); // will free movement and movement->request
                sem_post(fileio_lock);
                lift_origin[this_lift_no-1] = destination;
            }
            else printf("[ERROR] Tried to process invalid request.\n");
            #ifdef DEBUG
            sem_wait(fileio_lock);
            printf("[DEBUG] Lift %d is now at %d\n", this_lift_no, lift_origin[this_lift_no-1]);
            printf("[DEBUG] This is lift %d completing\n", this_lift_no);
            sem_post(fileio_lock);
            #endif
            sem_post(buffer_lock);
        }
    }
    return NULL;
}
int main(int argc, char const *argv[])
{
    if( argc != 3) printf("Usage:\n"
                            "\tlift_sim_A m t\n"
                            "\t\tm = Size of buffer to be used by lifts\n"
                            "\t\tt = net time taken to travel for all lifts\n");
    else
    {
        pid_t ptid, ptidf;
        int pstat = 0;
        int m = atoi(argv[1]);
        const int t = atoi(argv[2]);
        if (t >= 0 && m >=0 )
        {

            printf("[INFO] Initializing simulation using buffer size %d elements and lift time of %d seconds\n", m, t);
            // initialize shared memory for buffer and semaphores
            //0x20 for MAP_ANONYMOUS my IDE not registering association
            fileio_lock = (sem_t*)mmap(NULL, sizeof(fileio_lock), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
            buffer_lock = (sem_t*)mmap(NULL, sizeof(buffer_lock), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
            req_buffer = (Req**)mmap(NULL, sizeof(req_buffer), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0); 
            cond = (sem_t*)mmap(NULL, sizeof(cond), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
            running = (int*)mmap(NULL, sizeof(running), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
            *running = 1;
            // init_mem_fork();
            if(sem_init(buffer_lock, 1, 1) == -1 && sem_init(fileio_lock, 1, 1) == -1 && sem_init(cond, 1, 1) == -1)
            {
                printf("[ERROR] Initialization failed.\n");
            } 
            else
            {
                ptid = fork();
                if (ptid == 0)
                {
                    lift_r((void*)&m);
                    exit(SUCC_EXIT_STAT);
                }
                else if( ptid < 0 ) printf("[ERROR] Creating Lift_R failed: Terminating\n"); // create lift_r
                else // main task
                {
                    printf("[INFO] Successfully created Lift R\n");
                    for(int i = 1; i < NUM_THREADS; i++) // i from 1 to 3 to create lift 1,2 & 3
                    {
                        struct lift_params* params = (struct lift_params*)calloc(1, sizeof(struct lift_params));
                        params->lift_no=i;
                        params->lift_time=t;
                        ptid = fork();
                        if (ptid == 0) //child
                        {
                            lift((void*)params); 
                            exit(SUCC_EXIT_STAT);
                        }
                        else if (ptid > 0) printf("[INFO] Successfully created lift %d\n", i); // parent
                        else printf("[ERROR] Creating Lift %d failed.\n", i);  //failure
                    }
                    // wait for all children to finish (no more children tasks to wait for)
                    printf("[INFO] Waiting for lifts to complete.");
                    do{
                        ptidf = wait(&pstat);
                    }
                    while(ptidf > 0); //until error core as a result of no children
                    // cleanup for parent only
                    free(req_buffer);
                    sem_destroy(fileio_lock);
                    sem_destroy(buffer_lock);
                }
            }
            // munmap(ptr, len)
        }
        else
        {
            printf("Usage:\n"
                            "\tlift_sim_A m t\n"
                            "\t\tm = Size of buffer to be used by lifts\n"
                            "\t\tt = net time taken to travel for all lifts\n"
                    "!Caution! Buffer size and time need to be positive integers!\n");

        }
    }
    return 0;
}
