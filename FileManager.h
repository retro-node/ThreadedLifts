#ifndef FILEMANAGER_H
#define FILEMANAGER_H
typedef struct {
    int start;
    int end;
} Instructions;
#define FILE_IN "sim_in"
#define FILE_OUT "sim_out"
Instructions* request(int);
int write_request(int, int);
#endif
