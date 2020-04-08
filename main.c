#include "main.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    #ifdef DEBUG
    printf("You're in debug mode\n");
    #endif
    #ifdef UNIT_TEST
    printf("You're in unit test mode\n"
    "1. FileManager\n"
    "Select a unit to test: ");
    int opt;
    scanf("%d", &opt);
    if(opt == 0 )
    {
        // nothing
    }
    else if (opt == 1) 
    {
        unit_test_file_manager();
    }
    else
    {
        printf("test doesnt exist\n");
    }
    
    #endif


    printf("Stopping Simulation...\n");
    printf("Thank you for using ThreadedLifts\n");
    return 0;
}

#ifdef UNIT_TEST
void unit_test_file_manager(void)
{
    printf("FILEMANAGER UNIT TEST:\n"
    "1. request (sim_in)\n"
    "2. write_request (sim_out)\n"
    "Select: ");
    int opt;
    scanf("%d", &opt);
    if(opt == 0 )
    {
        /* Nothing */
    }
    else if(opt == 1)
    {
        int n ;
        printf("Line to read: ");
        scanf("%d", &n);
        Instructions* result = request(n);
        printf("Line %d: %d %d\n", n, result->start, result->end);
        free(result);
    }
    else if (opt == 2)
    {
        int start, end;
        printf("Starting floor: ");
        scanf("%d", &start);
        printf("\nEnding floor: ");
        scanf("%d", &end);
        write_request(start, end);
        printf("\nDone, check file. \n");
    }
    else
    {
        printf("test doesnt exist\n");
    }
    
}
#endif