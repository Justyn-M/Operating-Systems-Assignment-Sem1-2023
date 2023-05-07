// list_gen.c - generate a random list of customers and service type for c_file

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_list()
{
    // declaring variables
    int cust_num;
    char service_type;
    // seed random number generator for list
    srand(time(NULL));
    // write to c_file
    FILE *fp = fopen("c_file.txt", "w");

    //generate the 100 random customers for simulation
    for (int i = 1; i <= 100; i++) {
        cust_num = rand() % 3;
        // service types randomly assigned to each customer
        if (cust_num == 0) {
            service_type = 'W';
        } else if (cust_num == 1) {
            service_type = 'D';
        } else {
            service_type = 'I';
        }
    fprintf(fp, "%d %c\n", i, service_type);
    }
    fclose(fp);
    return 0;
}

