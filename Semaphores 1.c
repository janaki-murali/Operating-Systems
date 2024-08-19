#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define n 4
#define NIL 127

char arr[n];
pthread_mutex_t mutex;
pthread_cond_t empty;
pthread_cond_t full;
int empty_count = n;  // Number of empty slots in the buffer
int full_count = 0;   // Number of filled slots in the buffer

void initialize() {
    for (int i = 0; i < n; i++) {
        arr[i] = NIL;
    }
}

void display() {
    for (int i = 0; i < n; i++) {
        if (arr[i] == NIL) {
            printf("_ ");
        } else if (arr[i] > 90) {
            printf("%c ", arr[i]);
        } else {
            printf("%d ", arr[i]);
        }
    }
    printf("\n\n");
}

void* producer1(void* arg) {
    int num = 0;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (empty_count == 0) {
            pthread_cond_wait(&empty, &mutex);
        }
        
        // Produce an item
        for (int i = 0; i < n; i++) {
            if (arr[i] == NIL) {
                arr[i] = num;
                num++;
                empty_count--;
                full_count++;
                break;
            }
        }

        printf("PUT:%d Buffer:", num - 1);
        display();
        
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
    pthread_exit(NULL);
}

void* producer2(void* arg) {
    int ch = 'a';
    while (1) {
        pthread_mutex_lock(&mutex);
        while (empty_count == 0) {
            pthread_cond_wait(&empty, &mutex);
        }
        
        // Produce an item
        for (int i = 0; i < n; i++) {
            if (arr[i] == NIL) {
                arr[i] = ch;
                ch++;
                empty_count--;
                full_count++;
                break;
            }
        }

        printf("PUT:%c Buffer:", ch - 1);
        display();
        
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (full_count == 0) {
            pthread_cond_wait(&full, &mutex);
        }
        
        // Consume an item
        int num;
        for (int i = 0; i < n; i++) {
            if (arr[i] != NIL) {
                num = arr[i];
                arr[i] = NIL;
                full_count--;
                empty_count++;
                break;
            }
        }

        if (num > 90) {
            printf("GET:%c Buffer:", num);
        } else {
            printf("GET:%d Buffer:", num);
        }

        display();
        
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t p1, p2, c;
    pthread_attr_t attr;

    // Initialize synchronization primitives
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);

    initialize();
    pthread_attr_init(&attr);
    
    // Create producer and consumer threads
    pthread_create(&p1, &attr, producer1, NULL);
    pthread_create(&p2, &attr, producer2, NULL);
    pthread_create(&c, &attr, consumer, NULL);
    
    // Join threads (infinite loop, so this line will never be reached)
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(c, NULL);
    
    // Cleanup (this will never be reached due to infinite loop)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);

    return 0;
}
