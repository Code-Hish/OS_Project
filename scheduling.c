#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 100

// Structure to represent a process
typedef struct {
    int pid;        // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
    int completion; // Completion time
    int remaining;  // Remaining burst time for Round Robin
    int processed;  // Flag to check if the process has been added to the queue (for Round Robin)
} Process;

// Function prototypes
void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void roundRobin(Process processes[], int n, int quantum);
void calculateMetrics(Process processes[], int n);
void printMetrics(Process processes[], int n, const char *algorithm);

int main() {
    system("cls");
    int n, choice, quantum;
    int i;
    Process processes[MAX_PROCESSES];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time for process P%d: ", processes[i].pid);
        scanf("%d", &processes[i].arrival);
        printf("Enter burst time for process P%d: ", processes[i].pid);
        scanf("%d", &processes[i].burst);
        processes[i].remaining = processes[i].burst; // Set remaining burst time
        processes[i].processed = 0; // Initialize the processed flag
    }

    while (1) {
        printf("\nCPU Scheduling Algorithms:\n");
        printf("1. First-Come-First-Serve (FCFS)\n");
        printf("2. Shortest Job First (SJF)\n");
        printf("3. Round Robin (RR)\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(processes, n);
                break;
            case 2:
                sjf(processes, n);
                break;
            case 3:
                printf("Enter time quantum for Round Robin: ");
                scanf("%d", &quantum);
                roundRobin(processes, n, quantum);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// First-Come-First-Serve Scheduling
void fcfs(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    int i, j;
    for (i = 0; i < n; i++) {
        temp[i] = processes[i];
    }

    // Sort by arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (temp[j].arrival > temp[j + 1].arrival) {
                Process t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    calculateMetrics(temp, n);
    printMetrics(temp, n, "First-Come-First-Serve (FCFS)");
}

// Shortest Job First Scheduling
void sjf(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    int i, j;
    for (i = 0; i < n; i++) {
        temp[i] = processes[i];
    }

    // Sort by arrival time, then by burst time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (temp[j].arrival > temp[j + 1].arrival ||
                (temp[j].arrival == temp[j + 1].arrival && temp[j].burst > temp[j + 1].burst)) {
                Process t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    calculateMetrics(temp, n);
    printMetrics(temp, n, "Shortest Job First (SJF)");
}

// Round Robin Scheduling
void roundRobin(Process processes[], int n, int quantum) {
    Process temp[MAX_PROCESSES];
    int i;
    for (i = 0; i < n; i++) {
        temp[i] = processes[i];
        temp[i].remaining = processes[i].burst; // Reset remaining burst time for RR
        temp[i].processed = 0; // Reset the processed flag
    }

    int currentTime = 0;
    int remainingProcesses = n;
    int queue[MAX_PROCESSES];
    int front = 0, rear = 0;

    // Add processes to the queue that are ready to execute
    for (i = 0; i < n; i++) {
        if (temp[i].arrival <= currentTime) {
            queue[rear++] = i;
            temp[i].processed = 1; // Mark as processed
        }
    }

    // Process queue in round robin fashion
    while (remainingProcesses > 0) {
        int idx = queue[front++];
        if (temp[idx].remaining > quantum) {
            temp[idx].remaining -= quantum;
            currentTime += quantum;
        } else {
            currentTime += temp[idx].remaining;
            temp[idx].remaining = 0;
            remainingProcesses--;
        }

        // Check if any new process has arrived while processing
        for (i = 0; i < n; i++) {
            if (temp[i].arrival <= currentTime && !temp[i].processed) {
                queue[rear++] = i;
                temp[i].processed = 1; // Mark as processed
            }
        }

        // Add back the process to queue if it still has remaining burst time
        if (temp[idx].remaining > 0) {
            queue[rear++] = idx;
        }
    }

    // Calculate metrics after Round Robin scheduling
    calculateMetrics(temp, n);
    printMetrics(temp, n, "Round Robin (RR)");
}

// Calculate metrics: waiting time, turnaround time, and completion time
void calculateMetrics(Process processes[], int n) {
    int currentTime = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }
        processes[i].completion = currentTime + processes[i].burst;
        processes[i].turnaround = processes[i].completion - processes[i].arrival;
        processes[i].waiting = processes[i].turnaround - processes[i].burst;
        currentTime = processes[i].completion;
    }
}

// Print metrics and performance
void printMetrics(Process processes[], int n, const char *algorithm) {
    int totalWaiting = 0, totalTurnaround = 0;
    double throughput = (double)n / processes[n - 1].completion;
    int i;

    printf("\n%s:\n", algorithm);
    printf("\nP\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrival,
               processes[i].burst,
               processes[i].completion,
               processes[i].turnaround,
               processes[i].waiting);

        totalWaiting += processes[i].waiting;
        totalTurnaround += processes[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f\n", (double)totalWaiting / n);
    printf("Average Turnaround Time: %.2f\n", (double)totalTurnaround / n);
    printf("Throughput: %.2f processes/unit time\n", throughput);
}
