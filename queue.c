#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "queue.h"
#include "macros.h"

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