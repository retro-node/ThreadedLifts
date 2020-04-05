#include "main.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    
    #ifdef DEBUG
    printf("You're in debug mode\n");
    #endif
    request("sim_in.txt");
    return 0;
}