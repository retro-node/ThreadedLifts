/*************************
* FILE INPUT AND OUTPUT
* Handles reading simulator instructions from file and writing events to file specific to forked_lifts
* Author: Jeff Dugmore
* Date Forked: 05/04/2020
* Date Modified: 16/04/2020
************************/

#include "FileManagerForked.h"
static int* total_move_no[3];
static int* reqNo;
static int* lineNo = 0;

/******************
* init_mem_fork
* Initializes tracking variables into shared memory.
*****************/
void init_mem_fork(void)
{
    *total_move_no = (int*)mmap(NULL, sizeof(*total_move_no), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    reqNo = (int*)mmap(NULL, sizeof(*reqNo), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    lineNo = (int*)mmap(NULL, sizeof(*lineNo), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
}

/******************
* request
* Loads sim input file into a buffer line by line.
*****************/
void* request(void)
{
    char* filename = "sim_in";
    FILE* fp = NULL;
    Req* out_request = NULL;
    fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        printf("File not found, cannot load sim!\n");
    }
    else
    {
        int source = 0, dest = 0;
        int res;
        out_request = (Req*)calloc(1, sizeof(Req));
        for (int i = 0; i <= (*lineNo); i++) // scan (*lineNo) amount of lines first
        {
            res = fscanf(fp, "%d %d\n", &source, &dest); 
        }
        if(res != EOF)
        {

            if(res == 2 && ((source > 0 && source <= 20) && (dest > 0 && dest <= 20))) // atoi avoid chars
            {
                out_request -> source = source;
                out_request -> dest = dest;
                #ifdef DEBUG
                printf("[INFO] Loading instructions: floor %d --> floor %d\n", source, dest);
                #endif
            }
            else
            {
                #ifdef DEBUG
                printf("Source: %d | Destination: %d\n", source, dest);
                #endif
                printf("[WARNING] Invalid format in sim file. Line %d\n"
                "Please ensure floors are between 1 and 20.\n", (*lineNo)+1);
                if(ferror(fp))
                {
                    perror("Error occured while attempting to read file\n");
                }
                out_request = NULL;
            }
            (*lineNo)++;
        }
        else
        {
            printf("No more lines to load...\n");
            out_request = NULL;
        }
        
        fclose(fp);

    }
    return (void*)out_request;
}

/******************
 * write_request 
 * Writes a request to file sim_out for logging.
 * Requires: floor numbers as ints, atomic excecution.
 * Returns: request number to be needed for total req count.
 *****************/
int write_request(void* req)
{ 
    Req** request = (Req**)&req;
    
    FILE* fp = fopen(FILE_OUT, "ab");
    (*reqNo)++;
    (**request).req_no = (*reqNo);
    if(fp != NULL)
    {
        fprintf(fp, 
            "-------------------------------------------\n"
            "  New Lift Request From Floor %d to Foor %d\n"
            "  Request No: %d\n"    
            "-------------------------------------------\n", (**request).source, (**request).dest, (*reqNo));
        if(ferror(fp))
        {
            perror("Error occured while attempting to write request to file.\n");
        }
    }
    else printf("Something went wrong! Couldn't write request to file.\n");
    if(ferror(fp))
    {
        perror("Error occured while attempting to write request to file.\n");
    }
    fclose(fp);
    return (*reqNo);
}

/******************
 * write_completed
 * Writes a log of completed request to file sim_out.
 * Requires: floor numbers, request number, atomic excecution.
 * Returns: movement number to be needed for total move number.
 *****************/
int write_completed(void* mv)
{ 
    lift_move * move = (lift_move*)calloc(1, sizeof(lift_move));
    *move = *(lift_move*)mv;
    free(mv);
    
    int lift_no = move->lift_no;
    int source = move->request->source;
    int dest = move->request->dest;
    if(move->num_moves == 0) // Calculate the number of moves
    {
        if(!(source == 0 || dest == 0)){
            move->num_moves = abs(move->lift_origin - source) + abs(source - dest);
        }
        #ifdef DEBUG
        else
        {
            printf("[ERROR] Request did not contain either source or destination\n");
        }
        #endif
    }
    (*total_move_no)[lift_no] += move->num_moves; // add moves to total moves for this lift
    FILE* fp = fopen(FILE_OUT, "ab");
    if(fp != NULL)
    {
        fprintf(fp, 
            "-------------------------------------------\n"
            "Lift-%d Operation\n"
            "Previous position: Floor %d\n"
            "Request: Floor %d to Floor %d\n"
            "Detail operations:\n"
            "    Go from Floor %d to Floor %d\n"
            "    Go from Floor %d to Floor %d\n"
            "    #movement for this request: %d\n"
            "    #request: %d\n"
            "    Total #movement: %d\n"
            "Current position: Floor %d\n"
           "-------------------------------------------\n",
            move->lift_no,
            move->lift_origin,
            move->request->source, move->request->dest,
            move->lift_origin, move->request->source,
            move->request->source, move->request->dest,
            move->num_moves,
            move->request->req_no,
            (*total_move_no)[lift_no],
            move->request->dest);
        if(ferror(fp))
        {
            perror("Error occured while attempting to write successful action to file.\n");
        }
    }
    else
    {
        printf("Something went wrong! Couldn't write successful action to file.\n");
    }
    
    fclose(fp);
    free(move->request);
    free(move);
 
    return (*total_move_no)[lift_no];
}
