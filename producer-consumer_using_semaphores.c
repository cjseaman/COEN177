/*
 * Collin Seaman
 * COEN 177 Lab 5
 * 1/6/2018
 * Producer-Consumer with synchronization
 * This program implements a producer-consumer relationship between two threads, 
 * with shared data controlled by semaphores.
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <fcntl.h>

#define BUFF_SIZE 5
#define NCONSUMERS 2
#define NPRODUCERS 20

sem_t full; //num of buffer slots filled
sem_t empty; //num of buffer slots empty
sem_t mutex; //controls access to buffer
unsigned int producer_index = 0;
unsigned int consumer_index = 0;
int buffer[BUFF_SIZE] = {0};

pthread_t consumers[NCONSUMERS];
pthread_t producers[NPRODUCERS];

void *producer(void *);
void *consumer(void *);
//main
int main (int argc, char *argv[]) {
	int i;
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFF_SIZE);
	sem_init(&mutex, 0, 1);
	
	//create threads
	for(i = 0; i < NPRODUCERS; i++)
		pthread_create(&producers[i], NULL, consumer, (void *) (size_t) i);
	for(i = 0; i < NCONSUMERS; i++)
		pthread_create(&consumers[i], NULL, producer, (void *) (size_t) i);
	//wait for threads to terminate
	for(i = 0; i < NPRODUCERS; i++)
		pthread_join(producers[i], NULL);
	for(i = 0; i < NCONSUMERS; i++)
		pthread_join(consumers[i], NULL);
}
//Producer function, waits for space to open up in buffer array, decrements empty,
//and then inserts a random integer
//Then increments full
void* producer(void* id) {
	int value;
	do {
		value = (int) rand();
		sem_wait(&empty);
		sem_wait(&mutex);			
		buffer[producer_index] = value;
		printf("Producer %d | Produced item %d, stored in buff at index %d.\n", 
		id, value, producer_index);
		fflush(stdout);
		producer_index = (producer_index + 1)%BUFF_SIZE;
		sem_post(&mutex);
		sem_post(&full);
	} while(1);
}
//Consumer function, waits for some items to be added to buffer array, 
//then decrements full, and prints the value at that index, then 
//increments empty
void* consumer(void * id) {
	int value;
	do {
		sem_wait(&full);
		sem_wait(&mutex);
		value = buffer[consumer_index];
		printf("Consumer %d | Consumed item %d from buff index %d.\n", 
		id, value, consumer_index);
		fflush(stdout);
		consumer_index = (consumer_index + 1)%BUFF_SIZE;
		sem_post(&mutex);
		sem_post(&empty);
	} while (1);
}


