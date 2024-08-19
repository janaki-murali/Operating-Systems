#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 25

int n, r[MAX_REQUESTS], temp[MAX_REQUESTS], initial, size;

void sort_requests(int arr[], int n) {
    int i, j, temp_v;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp_v = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp_v;
            }
        }
    }
}

void fcfs() {
    int i, current = initial, seek = 0;
    for (i = 0; i < n; i++) {
        seek += abs(r[i] - current);
        current = r[i];
    }
    printf("FCFS Total head movement (seek time): %d\n", seek);
}

void sstf() {
    int i, j, min_index, current = initial, seek = 0, min_distance;
    int visited[MAX_REQUESTS] = {0};

    for (i = 0; i < n; i++) {
        min_distance = 1000000;
        for (j = 0; j < n; j++) {
            if (!visited[j] && abs(current - r[j]) < min_distance) {
                min_distance = abs(current - r[j]);
                min_index = j;
            }
        }
        visited[min_index] = 1;
        seek += min_distance;
        current = r[min_index];
    }

    printf("SSTF Total head movement (seek time): %d\n", seek);
}

void scan() {
    int i, current = initial, seek = 0, index;
    sort_requests(temp, n);

    for (i = 0; i < n; i++) {
        if (current < temp[i]) {
            index = i;
            break;
        }
    }
    for (i = index; i < n; i++) {
        seek += abs(temp[i] - current);
        current = temp[i];
    }
    seek += abs(size - 1 - current);
    current = size - 1;
    for (i = index - 1; i >= 0; i--) {
        seek += abs(current - temp[i]);
        current = temp[i];
    }
    printf("SCAN Total head movement (seek time): %d\n", seek);
}

void cscan() {
    int i, current = initial, seek = 0, index;
    sort_requests(temp, n);

    for (i = 0; i < n; i++) {
        if (current < temp[i]) {
            index = i;
            break;
        }
    }
    for (i = index; i < n; i++) {
        seek += abs(temp[i] - current);
        current = temp[i];
    }
    seek += abs(size - 1 - current);
    current = size - 1;
    seek += current;
    current = 0;
    for (i = 0; i < index; i++) {
        seek += abs(temp[i] - current);
        current = temp[i];
    }
    printf("CSCAN Total head movement (seek time): %d\n", seek);
}

void look() {
    int i, current = initial, seek = 0, index;
    sort_requests(temp, n);

    for (i = 0; i < n; i++) {
        if (current < temp[i]) {
            index = i;
            break;
        }
    }
    for (i = index; i < n; i++) {
        seek += abs(temp[i] - current);
        current = temp[i];
    }
    for (i = index - 1; i >= 0; i--) {
        seek += abs(current - temp[i]);
        current = temp[i];
    }
    printf("LOOK Total head movement (seek time): %d\n", seek);
}

void clook() {
    int i, current = initial, seek = 0, index;
    sort_requests(temp, n);

    for (i = 0; i < n; i++) {
        if (current < temp[i]) {
            index = i;
            break;
        }
    }
    for (i = index; i < n; i++) {
        seek += abs(temp[i] - current);
        current = temp[i];
    }
    seek += current - temp[0];
    current = temp[0];
    for (i = 1; i < index; i++) {
        seek += abs(current - temp[i]);
        current = temp[i];
    }
    printf("CLOOK Total head movement (seek time): %d\n", seek);
}

int main() {
    int i;
    printf("How many requests? : ");
    scanf("%d", &n);

    if (n > MAX_REQUESTS) {
        printf("Number of requests exceeds maximum allowed (%d).\n", MAX_REQUESTS);
        return 1;
    }

    printf("Enter the request sequence: ");
    for (i = 0; i < n; i++) {
        scanf("%d", &r[i]);
    }
    for (i = 0; i < n; i++) {
        temp[i] = r[i];
    }
    printf("Enter the total disk size: ");
    scanf("%d", &size);
    printf("Enter the initial head position: ");
    scanf("%d", &initial);

    fcfs();
    scan();
    cscan();
    look();
    clook();
    sstf();

    return 0;
}
