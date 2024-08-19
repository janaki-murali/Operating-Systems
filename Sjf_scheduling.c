#include <stdio.h>

struct Process {
    int pid;
    int at;  // Arrival time
    int bt;  // Burst time
    int ct;  // Completion time
    int tt;  // Turnaround time
    int wt;  // Waiting time
    int completed;
};

int n; // Number of processes
struct Process p[100]; // Array to store processes

void sjf() {
    int totalburst = 0, currenttime = 0, completed = 0;
    float avg_wt = 0, avg_tt = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input process details
    for (int i = 0; i < n; i++) {
        printf("Enter process_id, arrival_time, and burst_time in order: ");
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].completed = 0;
        totalburst += p[i].bt;
    }

    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    while (completed != n) {
        int min_index = -1;
        int minimum = totalburst + 1; // Set to a value larger than any possible burst time

        for (int i = 0; i < n; i++) {
            if (p[i].at <= currenttime && p[i].completed == 0) {
                if (p[i].bt < minimum) {
                    minimum = p[i].bt;
                    min_index = i;
                }
            }
        }

        if (min_index == -1) {
            currenttime++;
        } else {
            p[min_index].ct = currenttime + p[min_index].bt;
            p[min_index].tt = p[min_index].ct - p[min_index].at;
            p[min_index].wt = p[min_index].tt - p[min_index].bt;
            p[min_index].completed = 1;
            completed++;
            currenttime += p[min_index].bt;

            avg_tt += p[min_index].tt;
            avg_wt += p[min_index].wt;
        }
    }

    // Display process details
    printf("PID\tARR\tBURST\tCOMP\tTURN\tWAIT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tt, p[i].wt);
    }

    avg_wt /= n;
    avg_tt /= n;
    printf("Average Turnaround time: %f\n", avg_tt);
    printf("Average Waiting time: %f\n", avg_wt);
}

int main() {
    sjf();
    return 0;
}
