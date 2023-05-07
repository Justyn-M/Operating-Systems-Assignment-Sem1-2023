#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "macros.h"

// global variables
int total_customers = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// == Structs for Simulation == //

// customer structs
typedef struct _customer
{
    int c_number;
    char service;
    time_t customer_arrival;
} _customer;

// node struct for customer queue linked list
typedef struct node
{
    _customer data;
    struct node *next;
} node;

// FIFO queue struct
typedef struct c_queue
{
    node *head;
    node *tail;
} c_queue;

// teller struct
typedef struct _teller
{
    pthread_t teller_ID;
    time_t first_customer_time;
    int teller_number;
    int teller_terminate;
    c_queue *queue;
} _teller;


// == Customer Function == //
void customer(c_queue *queue)
{
    // declare all variables
    char line[MAX_LINE_LENGTH];
    int enqueue_delay;
    // Getting the list of customers from c_file
    FILE *fp = fopen("c_file.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    srand(time(NULL));
    // Extracting customer and service
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL ) // or no != NULL
    {
        _customer new_customer;
        if (sscanf(line, "%d %c", &new_customer.c_number, &new_customer.service) == 2)
        {
            enqueue(queue, new_customer);
            // generating a random delay on enqueue
            int enqueue_delay = rand() % 5;
            sleep(enqueue_delay);
        }
    }
    fclose(fp);
}

