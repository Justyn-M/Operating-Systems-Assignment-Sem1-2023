#ifndef QUEUE_H
#define QUEUE_H

#include <time.h>

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

// queue functions
int isEmpty(c_queue *queue);
void enqueue(c_queue *queue, _customer data);
_customer dequeue(c_queue *queue);

#endif