#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "teller.h"
#include "macros.h"

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