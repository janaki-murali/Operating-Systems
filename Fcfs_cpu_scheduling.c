#include <stdio.h>
#include <stdlib.h>

struct process {
    int at;  // Arrival time
    int bt;  // Burst time
    int tt;  // Turnaround time
    int wt;  // Waiting time
    int ct;  // Completion time
    int pid; // Process ID
    int st;  // Start time
    int rt;  // Remaining time
} p[10], temp;

int n;
int i, j, total;

void fcfs() {
    total = 0;
    float avg_wt = 0, avg_tt = 0;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        printf("\nEnter the pid, arrival time, burst time in order: ");
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;  // Initialize remaining time with burst time
    }
    
    // Sorting processes based on arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    
    for (i = 0; i < n; i++) {
        if (total < p[i].at) {
            total = p[i].at;  // If CPU is idle, move to the arrival time of the next process
        }
        
        // Calculating start time, completion time, turnaround time, and waiting time
        p[i].st = total;               // Start time is the current total time
        p[i].ct = total + p[i].bt;     // Completion time is start time + burst time
        total = p[i].ct;               // Update total to the completion time of the current process
        p[i].tt = p[i].ct - p[i].at;   // Turnaround time = completion time - arrival time
        p[i].wt = p[i].tt - p[i].bt;   // Waiting time = turnaround time - burst time
    }
    
    printf("\nPID | ARR | BURST | TURN | COMP | WAIT \n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].tt, p[i].ct, p[i].wt);
        avg_wt += p[i].wt;
        avg_tt += p[i].tt;
    }
    
    printf("\nAverage waiting time: %.2f", avg_wt / n);
    printf("\nAverage turnaround time: %.2f", avg_tt / n);
}

int main() {
    fcfs();
    return 0;
}
