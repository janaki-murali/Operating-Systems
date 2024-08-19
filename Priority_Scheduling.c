#include <stdio.h>

struct Process {
    int pid;
    int at; // Arrival time
    int bt; // Burst time
    int prio; // Priority
    int st; // Start time
    int ct; // Completion time
    int tt; // Turnaround time
    int wt; // Waiting time
    int completed;
};

void priority() {
    int n;
    float avg_wt = 0, avg_tt = 0;

    printf("\nEnter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n], temp;

    // Input process details
    for (int i = 0; i < n; i++) {
        printf("\nEnter PID, arrival time, burst time, and priority in order: ");
        scanf("%d %d %d %d", &p[i].pid, &p[i].at, &p[i].bt, &p[i].prio);
        p[i].completed = 0;
    }

    // Sort processes based on arrival time (if needed)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int completed = 0, currentTime = p[0].at; // Initialize currentTime with the arrival time of the first process

    // Execute processes
    while (completed != n) {
        int minPriority = 100000;
        int minIndex = -1;

        // Find the process with the highest priority available at current time
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && !p[i].completed && p[i].prio < minPriority) {
                minPriority = p[i].prio;
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
        } else {
            p[minIndex].st = currentTime;
            p[minIndex].ct = p[minIndex].st + p[minIndex].bt;
            p[minIndex].tt = p[minIndex].ct - p[minIndex].at;
            p[minIndex].wt = p[minIndex].tt - p[minIndex].bt;
            completed++;
            p[minIndex].completed = 1;
            currentTime = p[minIndex].ct;
            avg_wt += p[minIndex].wt;
            avg_tt += p[minIndex].tt;
            printf("|P%d|%d", p[minIndex].pid, p[minIndex].ct);
        }
    }

    // Output results
    printf("\nPID\tPRIOR\tARR\tBURST\tCOMP\tTURN\tWAIT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].prio, p[i].at, p[i].bt, p[i].ct, p[i].tt, p[i].wt);
    }

    // Output averages
    printf("\nAverage waiting time: %f", avg_wt / n);
    printf("\nAverage turnaround time: %f", avg_tt / n);
}

int main(){
    priority();
    return 0;
}
