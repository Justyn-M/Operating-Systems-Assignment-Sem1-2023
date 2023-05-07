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

// == Functions for FIFO Queue c_queue == //
// FIFO queue structure learned from DSA unit.

// Check if queue is empty
int isEmpty(c_queue *queue)
{
    return queue->head == NULL;
}
// Dont need isFull function as queue is unbounded

// enqueue function
void enqueue(c_queue *queue, _customer data)
{
    // declare variables
    char buffer[TIME_BUFFER];
    // create a new customer node
    node *new_cust_node = malloc(sizeof(node));
    new_cust_node->next = NULL;
    new_cust_node->data = data;
    new_cust_node->data.customer_arrival = time(NULL);
    
    
    if (queue->head == NULL)
    {
        queue-> head = new_cust_node;
    }
    else // if (queue->head != NULL)
    {
        queue ->tail ->next = new_cust_node;
    }
    queue->tail = new_cust_node;

    // Writing Customer Statistics to r_log
    FILE *fp = fopen("r_log.txt", "a");
    fprintf(fp, "====================\n");
    fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
    strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", 
        localtime(&new_cust_node->data.customer_arrival));
    fprintf(fp, "%s", buffer);
    fprintf(fp, "====================\n");
    fclose(fp);
}

// dequeue function
_customer dequeue(c_queue *queue)
{
    // Check if queue is empty because there is something wrong if it is empty
    if (queue-> head == NULL)
    {
        printf("Empty Queue\n");
        exit(1);
    }

    node *temp = queue->head;
    _customer data = temp->data;
    queue->head = temp->next;
    // Ensure that operations on c_queue are handled appropriately if queue is empty
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    free(temp);
    return data;
}