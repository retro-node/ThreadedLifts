#include "testing.h"

int main(int argc, char *argv[])
{
    printf("UNIT TESTING\n"
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
    printf("Stopping testing...\n");
    return 0;
}

void unit_test_file_manager(void)
{        
    int stop = 0;
    do {
        Req* req = NULL;
        printf("FILEMANAGER UNIT TEST:\n"
        "0. exit\n"
        "1. request (sim_in)\n"
        "2. write_request (sim_out)\n"
        "3. write_completed (sim_out)\n"
        "Select: ");
        int opt = 0;
        scanf("%d", &opt);
        if(opt <= 0 )
        {
            stop = 1;
        }
        else if(opt == 1)
        {
            printf("Reading next line: ");
            Req* result = request();
            printf("Line:%d %d\n", result->source, result->dest);
            free(result);
        }
        else if (opt == 2)
        {
            int start, end;
            printf("Starting floor: ");
            scanf("%d", &start);
            printf("\nEnding floor: ");
            scanf("%d", &end);
            req = (Req*)calloc(1, sizeof(Req));
            req->source = start;
            req->dest = end;
            write_request(req);
            printf("\nDone, check file. \n");
            free(req);
        }
        else if(opt == 3)
        {
            int lift_no, origin, start, end;
            if(req == NULL)
            {
                req = (Req*)calloc(1, sizeof(Req));
                printf("Starting floor: ");
                scanf("%d", &start);
                printf("\nEnding floor: ");
                scanf("%d", &end);
                req->source = start;
                req->dest = end;
            }
            lift_move* this_move = (lift_move*)calloc(1, sizeof(lift_move));
            printf("Lift No: ");
            scanf("%d", &lift_no);
            printf("original floor: ");
            scanf("%d", &origin);
            this_move->lift_no = lift_no;
            this_move->request = req;
            this_move->lift_origin = origin;
            this_move->request->req_no = write_request(req);
            write_completed(this_move);
        }
        else
        {
            printf("test doesnt exist\n");
        }
    } while(stop == 0);
}
