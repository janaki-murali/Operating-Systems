#include <stdio.h>
#include <stdlib.h>

#define MAXPAGES 100
#define MAXFRAMES 100

int frames[MAXFRAMES];

void display(int capacity) {
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == -1) {
            printf("[-] ");
        } else {
            printf("[%d] ", frames[i]);
        }
    }
    printf("\n");
}

void fifo(int referencestring[], int n, int capacity) {
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
    }

    int pagefault = 0;
    int frameindex = 0;

    printf("\nFIFO PAGE REPLACEMENT\n");

    for (int i = 0; i < n; i++) {
        int pageexists = 0;
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == referencestring[i]) {
                pageexists = 1;
                break;
            }
        }

        if (!pageexists) {
            frames[frameindex] = referencestring[i];
            frameindex = (frameindex + 1) % capacity;
            pagefault++;
        }

        display(capacity);
    }

    printf("Number of page faults: %d\n", pagefault);
}

void lru(int referencestring[], int n, int capacity) {
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
    }

    int pagefault = 0;
    int recent[MAXFRAMES];

    for (int i = 0; i < capacity; i++) {
        recent[i] = -1;
    }

    printf("\nLRU PAGE REPLACEMENT\n");

    for (int i = 0; i < n; i++) {
        int pageexists = 0;
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == referencestring[i]) {
                pageexists = 1;
                recent[j] = i;
                break;
            }
        }

        if (!pageexists) {
            int index = 0;
            for (int k = 1; k < capacity; k++) {
                if (recent[k] < recent[index]) {
                    index = k;
                }
            }
            frames[index] = referencestring[i];
            recent[index] = i;
            pagefault++;
        }

        display(capacity);
    }

    printf("Number of page faults: %d\n", pagefault);
}

void lfu(int referencestring[], int n, int capacity) {
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
    }

    int pagefault = 0;
    int frequency[MAXFRAMES] = {0};

    printf("\nLFU PAGE REPLACEMENT\n");

    for (int i = 0; i < n; i++) {
        int pageexists = 0;
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == referencestring[i]) {
                pageexists = 1;
                frequency[j]++;
                break;
            }
        }

        if (!pageexists) {
            int minfreqindex = 0;
            for (int k = 1; k < capacity; k++) {
                if (frames[k] == -1 || frequency[k] < frequency[minfreqindex]) {
                    minfreqindex = k;
                }
            }
            frames[minfreqindex] = referencestring[i];
            frequency[minfreqindex] = 1;  // Reset frequency for the new page
            pagefault++;
        }

        display(capacity);
    }

    printf("Number of page faults: %d\n", pagefault);
}

int main() {
    int n, referencestring[MAXPAGES], capacity;

    printf("How many pages in the reference string: ");
    scanf("%d", &n);

    printf("Enter the reference string: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &referencestring[i]);
    }

    printf("Enter the capacity of the memory: ");
    scanf("%d", &capacity);

    fifo(referencestring, n, capacity);
    lru(referencestring, n, capacity);
    lfu(referencestring, n, capacity);

    return 0;
}
