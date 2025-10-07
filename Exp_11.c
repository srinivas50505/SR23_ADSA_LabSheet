#include <stdio.h>

int maxProfit = 0;
int n;          // Number of items

void knapsackBacktrack(int i, int profit, int weight, int W, int wt[], int val[]) {
    if (i == n) {
        if (profit > maxProfit)
            maxProfit = profit;
        return;
    }

    // Exclude the current item
    knapsackBacktrack(i + 1, profit, weight, W, wt, val);

    // Include the current item if it doesn't exceed capacity
    if (weight + wt[i] <= W) {
        knapsackBacktrack(i + 1, profit + val[i], weight + wt[i], W, wt, val);
    }
}

int main() {
    int W = 50; // Capacity of knapsack
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};

    n = sizeof(val) / sizeof(val[0]);

    knapsackBacktrack(0, 0, 0, W, wt, val);

    printf("Maximum profit using Backtracking = %d\n", maxProfit);

    return 0;
}
