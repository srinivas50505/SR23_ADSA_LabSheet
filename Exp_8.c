#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;       // Job ID
    int deadline; // Deadline by which job should be done
    int profit;   // Profit for completing the job
} Job;

// Compare function to sort jobs based on profit in descending order
int compareJobs(const void* a, const void* b) {
    Job* job1 = (Job*)a;
    Job* job2 = (Job*)b;
    return job2->profit - job1->profit;
}

// Job sequencing function
void jobSequencing(Job jobs[], int n) {
    // Sort jobs by descending profit
    qsort(jobs, n, sizeof(Job), compareJobs);

    int result[n];  // To store result (sequence of jobs)
    int slot[n];    // To keep track of free time slots

    for (int i = 0; i < n; i++)
        slot[i] = 0;  // Initialize all slots as free

    for (int i = 0; i < n; i++) {
        // Find a free slot for this job, starting from its deadline
        for (int j = (jobs[i].deadline < n ? jobs[i].deadline : n) - 1; j >= 0; j--) {
            if (slot[j] == 0) {
                slot[j] = 1;     // Mark slot as occupied
                result[j] = i;   // Store job index
                break;
            }
        }
    }

    printf("Scheduled Jobs:\n");
    int totalProfit = 0;
    for (int i = 0; i < n; i++) {
        if (slot[i]) {
            printf("Job %d (Profit: %d, Deadline: %d)\n", jobs[result[i]].id, jobs[result[i]].profit, jobs[result[i]].deadline);
            totalProfit += jobs[result[i]].profit;
        }
    }
    printf("Total Profit: %d\n", totalProfit);
}

int main() {
    Job jobs[] = {
        {1, 4, 20},
        {2, 1, 10},
        {3, 1, 40},
        {4, 1, 30}
    };
    int n = sizeof(jobs) / sizeof(jobs[0]);

    jobSequencing(jobs, n);
    return 0;
}
