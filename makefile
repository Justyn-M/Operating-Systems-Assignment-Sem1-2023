CC = gcc
CFLAGS = -Wall -pthread
OBJS = cq.o customer.o queue.o teller.o
EXEC = cq

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

cq.o: cq.c queue.h customer.h teller.h
	$(CC) $(CFLAGS) -c cq.c

customer.o: customer.c queue.h customer.h
	$(CC) $(CFLAGS) -c customer.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

teller.o: teller.c queue.h teller.h
	$(CC) $(CFLAGS) -c teller.c

clean:
	rm -f $(EXEC) $(OBJS)