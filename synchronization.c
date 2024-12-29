#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_produce;
pthread_cond_t cond_consume;

// Function for the producer thread
void* producer(void* arg) {
    int item = 0;
    while (item != 6) {
        item++;

        pthread_mutex_lock(&mutex);

        // Wait if the buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_produce, &mutex);
        }

        // Produce an item
        buffer[count] = item;
        count++;
        printf("Producer: produced item %d\n", item);

        // Signal the consumer that an item is available
        pthread_cond_signal(&cond_consume);

        pthread_mutex_unlock(&mutex);

        sleep(1); // Simulate production time
    }
    return NULL;
}

// Function for the consumer thread
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&cond_consume, &mutex);
        }

        // Consume an item
        int item = buffer[count - 1];
        count--;
        printf("Consumer: consumed item %d\n", item);

        // Signal the producer that space is available
        pthread_cond_signal(&cond_produce);

        pthread_mutex_unlock(&mutex);

        sleep(1); // Simulate consumption time
        if(item == 6)
            break;
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_produce, NULL);
    pthread_cond_init(&cond_consume, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to finish (they won't in this example)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_produce);
    pthread_cond_destroy(&cond_consume);

    return 0;
}
