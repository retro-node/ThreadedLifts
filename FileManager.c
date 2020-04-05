#include "FileManager.h"
#include <stdio.h>

int main(int argc, char const *argv[]) //testing
{
    printf("Hello World!\n");
    #ifdef DEBUG
    printf("Youre in debug mode\n");
    #endif
    return 0;
}

void test(int i){
    printf("%d", i);
}
