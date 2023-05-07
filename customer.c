// customer.c - customer function that reads and enqueues customers from c_file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "customer.h"
#include "queue.h"
#include "macros.h"

// == Customer Function == //
void customer(c_queue *queue)
{
    // declare all variables
    char line[MAX_LINE_LENGTH];
    int enqueue_delay = rand() % 5;
    // Getting the list of customers from c_file
    FILE *fp = fopen("c_file.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    srand(time(NULL));
    // Extracting customer and service
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL )
    {
        _customer new_customer;
        if (sscanf(line, "%d %c", &new_customer.c_number, &new_customer.service) == 2)
        {
            enqueue(queue, new_customer);
            sleep(enqueue_delay);
        }
    }
    fclose(fp);
}