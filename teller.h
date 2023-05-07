#ifndef TELLER_H
#define TELLER_H

#include <pthread.h>
#include "queue.h"

extern int total_customers;
extern pthread_mutex_t mutex;

// teller struct
typedef struct _teller
{
    time_t first_customer_time;
    int teller_number;
    int teller_terminate;
    int count_customers;
    c_queue *queue;
} _teller;

void teller(_teller *curr);

#endif