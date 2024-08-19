#include <stdio.h>

#define MAX_PROCESSES 100

struct Process {
    int pid;       // Process ID
    int at;        // Arrival Time
    int bt;        // Burst Time
    int rem;       // Remaining Time
    int completed; // Completion Status
};

struct Process p[MAX_PROCESSES];

void roundrobin() {
    int n, slice, time = 0, remain, flag;
    int i, j;
    struct Process temp;
    float avg_wt = 0, avg_tt = 0;

    printf("\nEnter the number of processes: ");
    scanf("%d", &n);

    // Input process details
    for (i = 0; i < n; i++) {
        printf("\nEnter PID, arrival time, and burst time in order: ");
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].rem = p[i].bt; // Initialize remaining burst time
        p[i].completed = 0;
    }

    printf("\nEnter the time slice: ");
    scanf("%d", &slice);

    // Sort processes based on arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    remain = n;
    printf("\nPID\tARR\tBURST\tTURN\tCOMP\tWAIT\n");

    // Execute processes
    while (remain != 0) {
        flag = 0;
        for (i = 0; i < n; i++) {
            if (p[i].rem > 0 && p[i].at <= time) {
                if (p[i].rem <= slice) {
                    time += p[i].rem;
                    p[i].rem = 0;
                    flag = 1;
                } else {
                    time += slice;
                    p[i].rem -= slice;
                }
                if (p[i].rem == 0) {
                    remain--;
                    printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt,
                           time - p[i].at, time, time - p[i].at - p[i].bt);
                    avg_tt += time - p[i].at;
                    avg_wt += time - p[i].at - p[i].bt;
                }
            }
        }
        if (flag == 0) {
            time++; // Increment time if no process is running
        }
    }

    // Output averages
    printf("\nAverage turnaround time: %f", avg_tt / n);
    printf("\nAverage waiting time: %f", avg_wt / n);
}

int main() {
    roundrobin();
    return 0;
}
