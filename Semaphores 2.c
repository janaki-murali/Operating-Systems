#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h> 

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 10
#define NUM_CONSUMERS 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

int mutex = 1;      
int empty = BUFFER_SIZE; 
int full = 0;       
void wait(int *sem) {
    while (*sem <= 0) {
        sleep(1);
    }
    (*sem)--;
}

void my_signal(int *sem) {
    (*sem)++;
}

void* producer(void* arg) {
    int item;
    int producer_id = *(int*)arg; 
    while (1) {
        item = rand() % 100; 
        wait(&empty);
        wait(&mutex);
        buffer[in] = item;
        printf("Producer %d produced item: %d\n", producer_id, item);
        in = (in + 1) % BUFFER_SIZE;
        my_signal(&mutex);
        my_signal(&full);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    int consumer_id = *(int*)arg; 
    while (1) {
        wait(&full);
        wait(&mutex);
        item = buffer[out];
        printf("Consumer %d consumed item: %d\n", consumer_id, item);
        out = (out + 1) % BUFFER_SIZE;
        my_signal(&mutex);
        my_signal(&empty);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        int *producer_id = malloc(sizeof(int)); 
        *producer_id = i; 
        pthread_create(&producers[i], NULL, producer, producer_id);
    }

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        int *consumer_id = malloc(sizeof(int)); 
        *consumer_id = i;
        pthread_create(&consumers[i], NULL, consumer, consumer_id);
    }

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumers[i], NULL);
    }

    return 0;
}
