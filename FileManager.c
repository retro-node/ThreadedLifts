/*************************
* FILE INPUT AND OUTPUT
* Handles reading simulator instructions from file and writing events to file.
* Author: Jeff Dugmore
* Date Created: 05/04/2020
* Date Modified: 05/04/2020
************************/
#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>

/******************
* request
* Loads sim input file into a buffer line by line.
*****************/

Instructions* request(int lineNo)
{
    char* filename = "sim_in";
    FILE* fp = NULL;
    Instructions* out_instructions;
    fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        printf("File not found, cannot load sim!\n");
    }
    else
    {
        // static int lineNo = 0;
        int source = 0, dest = 0;
        int res;
        out_instructions = (Instructions*)calloc(1, sizeof(Instructions));
        for (int i = 0; i < lineNo; i++) // scan lineNo amount of lines first
        {
            res = fscanf(fp, "%d %d\n", &source, &dest); 
        }
        if(res != EOF)
        {

            if(res != 2 && ((source > 0 && source <= 20) || (dest > 0 && dest <= 20))) //determine basement and sublevels
            {
                #ifdef DEBUG
                printf("Source: %d | Destination: %d\n", source, dest);
                #endif
                printf("[WARNING] Invalid format in sim file. Line %d\n"
                "Please ensure no floors are between 1 and 20.\n", lineNo+1);
                if(ferror(fp))
                {
                    perror("Error occured while attempting to read file\n");
                }
            }
            else
            {
                out_instructions -> start = source;
                out_instructions -> end = dest;
                #ifdef DEBUG
                printf("out_instructions -> start: %d \nout_instructions -> end: %d\n", source, dest);
                #endif
            }
            lineNo++;
        }
        else{printf("No more lines to load...\n");}
        
        fclose(fp);
    }
    return out_instructions;
}

/******************
 * write_request 
 * Writes a request to file sim_out for logging.
 * Requires: floor numbers as ints.
 * Returns: request number to be discarded unless needed for total req count.
 *****************/
int write_request(int from, int to){ 
    static int reqNo;
    FILE* fp = fopen(FILE_OUT, "wb");
    reqNo++;
    if(fp != NULL)
    {
        fprintf(fp, 
            "-------------------------------------------\n"
            "  New Lift Request From Floor %d to Foor %d\n"
            "  Request No: %d\n"    
            "-------------------------------------------\n", from, to, reqNo);
        if(ferror(fp))
        {
            perror("Error occured while attempting to write to file.\n");
        }
    }
    else
    {
        printf("Something went wrong! Couldn't write request to file.\n");
    }
    
    fclose(fp);
    return reqNo;
}