#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond;
int state = 1;

void *thread_f(void *arg) {
    int no = *((int *)arg);
    int st = no;
    
    while (1) {
        pthread_mutex_lock(&lock);

        // Wait until the state matches the thread's expected state
        while (state != st) {
            pthread_cond_wait(&cond, &lock);
        }

        // Critical section
        printf("Thread [%d] -> %d\n", st, no);
        no += 2;
        state = 3 - st;  // Toggle between 1 and 2

        pthread_cond_signal(&cond);  // Wake up the other thread
        pthread_mutex_unlock(&lock);

        usleep(100000);  // Sleep for 100ms
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t id1, id2;
    int odd = 1;
    int even = 2;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&id1, NULL, thread_f, (void *)&odd);
    pthread_create(&id2, NULL, thread_f, (void *)&even);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
