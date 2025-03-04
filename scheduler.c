#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure for process
typedef struct {
    int pid;       // Process ID
    int arrival;   // Arrival Time
    int burst;     // Burst Time
    int waiting;   // Waiting Time
    int turnaround;// Turnaround Time
    int remaining; // Remaining Time (for SJF & RR)
    int completion;// Completion Time
} Process;

// Function to implement Shortest Job First (SJF) Scheduling
void sjf(Process *processes, int n) {
    int completed = 0, time = 0, min_index;
    printf("SJF Execution Order: ");
    while (completed < n) {
        min_index = -1;
        int min_time = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0 && processes[i].remaining < min_time) {
                min_time = processes[i].remaining;
                min_index = i;
            }
        }
        
        if (min_index == -1) {
            time++;
            continue;
        }
        
        printf("P%d ", processes[min_index].pid);
        processes[min_index].remaining--;
        time++;
        
        if (processes[min_index].remaining == 0) {
            completed++;
            processes[min_index].completion = time;
            processes[min_index].turnaround = processes[min_index].completion - processes[min_index].arrival;
            processes[min_index].waiting = processes[min_index].turnaround - processes[min_index].burst;
        }
    }
    printf("\n");
}

// Function to implement Round Robin (RR) Scheduling
void rr(Process *processes, int n, int quantum) {
    int time = 0, completed = 0;
    int queue[n], front = 0, rear = 0;
    int remaining_processes = n;
    for (int i = 0; i < n; i++) queue[i] = i; // Initialize queue
    
    printf("RR Execution Order: ");
    while (remaining_processes > 0) {
        int i = queue[front];
        front = (front + 1) % n;
        
        if (processes[i].remaining > 0 && processes[i].arrival <= time) {
            int exec_time = (processes[i].remaining > quantum) ? quantum : processes[i].remaining;
            processes[i].remaining -= exec_time;
            time += exec_time;
            printf("P%d ", processes[i].pid);
            
            if (processes[i].remaining == 0) {
                completed++;
                processes[i].completion = time;
                processes[i].turnaround = processes[i].completion - processes[i].arrival;
                processes[i].waiting = processes[i].turnaround - processes[i].burst;
                remaining_processes--;
            } else {
                queue[rear] = i;
                rear = (rear + 1) % n;
            }
        }
    }
    printf("\n");
}

// Function to calculate and print performance metrics
void print_metrics(Process *processes, int n) {
    float total_waiting = 0, total_turnaround = 0;
    printf("\nPID\tArrival\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].arrival, processes[i].burst, processes[i].waiting, processes[i].turnaround);
        total_waiting += processes[i].waiting;
        total_turnaround += processes[i].turnaround;
    }
    printf("\nAverage Waiting Time: %.2f", total_waiting / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
}

int main() {
    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process processes[n];
    
    printf("Enter Time Quantum for RR: ");
    scanf("%d", &quantum);
    
    printf("Enter process details (ID, Arrival Time, Burst Time):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &processes[i].pid, &processes[i].arrival, &processes[i].burst);
        processes[i].remaining = processes[i].burst;
        processes[i].waiting = 0;
        processes[i].turnaround = 0;
        processes[i].completion = 0;
    }
    
    // Run SJF and RR scheduling
    printf("\nExecuting SJF...\n");
    sjf(processes, n);
    print_metrics(processes, n);
    
    // Reset remaining times for RR execution
    for (int i = 0; i < n; i++) {
        processes[i].remaining = processes[i].burst;
    }
    
    printf("\nExecuting RR...\n");
    rr(processes, n, quantum);
    print_metrics(processes, n);
    
    return 0;
}
