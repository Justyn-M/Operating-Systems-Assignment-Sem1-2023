#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "macros.h"
#include "customer.h"
#include "queue.h"
#include "teller.h"
#include "list_gen.h"

// global variables
int total_customers = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// == Main Function == //
int main()
{
    // generate new list of customers
    generate_list();

    // create queue
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