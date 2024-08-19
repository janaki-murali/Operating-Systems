#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int finish[MAX_PROCESSES];

int num_processes, num_resources;

void initialize() {
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter the number of resources: ");
    scanf("%d", &num_resources);

    printf("Enter the available resources: ");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &available[i]);
    }

    printf("Enter the maximum demand of each process: \n");
    for (int i = 0; i < num_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    printf("Enter the allocation of each process: \n");
    for (int i = 0; i < num_processes; i++) {
        printf("For process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    for (int i = 0; i < num_processes; i++) {
        finish[i] = 0; // Initialize the finish array
    }
}

int safety_algorithm() {
    int work[MAX_RESOURCES];
    int safe_sequence[MAX_PROCESSES];
    int count = 0;

    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }

    // Reset finish array to 0
    for (int i = 0; i < num_processes; i++) {
        finish[i] = 0;
    }

    while (count < num_processes) {
        int found = 0;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == num_resources) { // If all resources are available
                    for (int k = 0; k < num_resources; k++) {
                        work[k] += allocation[i][k];
                    }
                    safe_sequence[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("The system is in an unsafe state.\n");
            return 0;
        }
    }

    printf("Safe sequence: ");
    for (int i = 0; i < num_processes; i++) {
        printf("%d ", safe_sequence[i]);
    }
    printf("\n");

    return 1;
}

int request_resources(int process_id, int request[]) {
    // Check if request exceeds need or available resources
    for (int i = 0; i < num_resources; i++) {
        if (request[i] > need[process_id][i]) {
            printf("The request exceeds the maximum demand.\n");
            return 0;
        }
        if (request[i] > available[i]) {
            printf("The request exceeds available resources.\n");
            return 0;
        }
    }

    // Pretend to allocate requested resources
    for (int i = 0; i < num_resources; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }

    // Check if the system is still in a safe state
    if (!safety_algorithm()) {
        // Rollback changes if not safe
        for (int i = 0; i < num_resources; i++) {
            available[i] += request[i];
            allocation[process_id][i] -= request[i];
            need[process_id][i] += request[i];
        }
        return 0;
    }

    return 1;
}

int main() {
    initialize();

    int process_id;
    int request[MAX_RESOURCES];

    printf("Enter the process ID for the resource request: ");
    scanf("%d", &process_id);

    printf("Enter the requested resources: ");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &request[i]);
    }

    if (request_resources(process_id, request)) {
        printf("The resources have been allocated safely.\n");
    } else {
        printf("The resources could not be allocated as it would lead to an unsafe state.\n");
    }

    return 0;
}
