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
    int count_customers;
    c_queue *queue;
} _teller;



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
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL ) // or no != NULL
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


// == Function for Teller == //
void teller(_teller *curr)
{
    c_queue *queue = curr->queue;
    while (1)
    {
        // locking mutex
        pthread_mutex_lock(&mutex);
        //  ==== Print to r_log when all customers are served ==== 
        if (total_customers == 100)
        {
            // To avoid deadlock, unlock the mutex
            pthread_mutex_unlock(&mutex);

            // Writing to r_log
            FILE *fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Served Customers: %d\n", curr->count_customers);

            // print time when first customer was served
            struct tm first_customer = *localtime(&curr->first_customer_time);
            char buffer[TIME_BUFFER];
            strftime(buffer, sizeof(buffer), "Start Time: %H:%M:%S\n", &first_customer);
            fprintf(fp, "%s", buffer);

            // Termination time
            time_t teller_end_time = time(NULL);
            struct tm tm = *localtime(&teller_end_time);
            fprintf(fp, "Terminating at %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);
            // Teller Termination Flag
            curr->teller_terminate = 1;
            // Exit thread because all customers are served
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex);

        // ==== Handling queue customers ==== //
        time_t _time = time(NULL);
        struct tm tm = *localtime(&_time);

        // Withdrawal Customer
        if (!isEmpty(queue) && queue->head->data.service == 'W')
        {
            _customer data = dequeue(queue);
            // Writing to r_log
            FILE *fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
            char buffer[TIME_BUFFER];
            strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", localtime(&data.customer_arrival));
            fprintf(fp, "%s", buffer);
            fprintf(fp, "Response Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);

            // Teller performs service
            sleep(7);
            pthread_mutex_lock(&mutex);
            total_customers++;
            curr->count_customers++;
            if (curr->first_customer_time == 0)
            {
                curr->first_customer_time = time(NULL);
            }
            pthread_mutex_unlock(&mutex);

            _time = time(NULL);
            tm = *localtime(&_time);
            // Writing to r_log
            fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
            strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", localtime(&data.customer_arrival));
            fprintf(fp, "%s", buffer);
            fprintf(fp, "Completion Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);
        }
        // Deposit Customer
        else if (!isEmpty(queue) && queue->head->data.service == 'D')
        {
            _customer data = dequeue(queue);
            // Writing to r_log
            FILE *fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
            char buffer[TIME_BUFFER];
            strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", localtime(&data.customer_arrival));
            fprintf(fp, "%s", buffer);
            fprintf(fp, "Response Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);

            // Teller performs service
            sleep(9);
            pthread_mutex_lock(&mutex);
            total_customers++;
            curr->count_customers++;
            if (curr->first_customer_time == 0)
            {
                curr->first_customer_time = time(NULL);
            }
            pthread_mutex_unlock(&mutex);

            _time = time(NULL);
            tm = *localtime(&_time);
            // Writing to r_log
            fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
            strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", localtime(&data.customer_arrival));
            fprintf(fp, "%s", buffer);
            fprintf(fp, "Completion Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);
        }
        // Information Customer
        else if (!isEmpty(queue) && queue->head->data.service == 'I')
        {
            _customer data = dequeue(queue);
            // Writing to r_log
            FILE *fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
            char buffer[TIME_BUFFER];
            strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", localtime(&data.customer_arrival));
            fprintf(fp, "%s", buffer);
            fprintf(fp, "Response Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);

            // Teller performs service
            sleep(12);
            pthread_mutex_lock(&mutex);
            total_customers++;
            curr->count_customers++;
            if (curr->first_customer_time == 0)
            {
                curr->first_customer_time = time(NULL);
            }
            pthread_mutex_unlock(&mutex);

            _time = time(NULL);
            tm = *localtime(&_time);
            // Writing to r_log
            fp = fopen("r_log.txt", "a");
            fprintf(fp, "====================\n");
            fprintf(fp, "Teller %d\n", curr->teller_number);
            fprintf(fp, "Customer %d : %c\n", data.c_number, data.service);
            strftime(buffer, TIME_BUFFER, "Arrival Time: %H:%M:%S\n", localtime(&data.customer_arrival));
            fprintf(fp, "%s", buffer);
            fprintf(fp, "Completion Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
            fprintf(fp, "====================\n");
            fclose(fp);
        }
        else
        {
            // wait until a customer arrives
            sleep(1);
        }
    }
}

// == Main Function == //
int main()
{
    c_queue queue = {NULL, NULL};
    // create 4 tellers
    pthread_t teller_threads[4];
    _teller tellers[4];

    // create customer thread
    pthread_t customer_thread;
    pthread_create(&customer_thread, NULL, (void *)customer, &queue);

    // starting teller threads
    for (int i = 0; i < 4; i++)
    {
        tellers[i].teller_number = i + 1;
        tellers[i].teller_terminate = 0;
        tellers[i].count_customers = 0;
        tellers[i].first_customer_time = 0;
        tellers[i].queue = &queue;
        pthread_create(&teller_threads[i], NULL, (void *)teller, &tellers[i]);
    }
    // join threads
    for (int i = 0; i < 4; i++)
    {
        pthread_join(teller_threads[i], NULL);
    }
    // print the teller statistics
    for (int i = 0; i < 4; i++)
    {
        if (tellers[i].teller_terminate == 1)
        {
            printf("Teller Statistics:\n");
            printf("Teller %d served %d customers\n", tellers[i].teller_number, tellers[i].count_customers);

        }
    }
    printf("Total customers served: %d\n", total_customers);
    pthread_join(customer_thread, NULL);
    return 0;
}