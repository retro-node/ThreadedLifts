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
void request(char* filename)
{
    FILE* fp = NULL;
    fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        printf("File not found, cannot load sim!\n");
    }
    else
    {
        int line = 1, stop = 0, res;
        
        do
        {
            int source = 0, dest = 0;
            res = fscanf(fp, "%d %d\n", &source, &dest);
            if(res == EOF) stop = 1;
            if(res != 2 && (source == 0 || dest == 0) && !stop)
            {
                #ifdef DEBUG
                printf("Source: %d | Destination: %d\n", source, dest);
                #endif
                printf("[WARNING] Invalid format in sim file. Line %d\n"
                "Please ensure no floors are labeled \"0\".\nUse"
                " -1 for basement and sublevels.\n", line);
                if(ferror(fp))
                {
                    perror("Error occured while attempting to read file");
                }
                
            }
            else
            {
                /* code */
            }
            
            line++;
        } while(!stop);
        fclose(fp);
    }
    
}
