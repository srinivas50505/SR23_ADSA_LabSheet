#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 4 // Number of vertices

int finalRes = INT_MAX;
int finalPath[V+1];

// Function to copy temporary solution to final solution
void copyToFinal(int currPath[]) {
    for (int i = 0; i < V; i++)
        finalPath[i] = currPath[i];
    finalPath[V] = currPath[0];
}

// Function to find minimum edge cost for a vertex
int firstMin(int cost[V][V], int i) {
    int min = INT_MAX;
    for (int k = 0; k < V; k++) {
        if (cost[i][k] < min && i != k)
            min = cost[i][k];
    }
    return min;
}

// Function to find second minimum edge cost for a vertex
int secondMin(int cost[V][V], int i) {
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < V; j++) {
        if (i == j) continue;
        if (cost[i][j] <= first) {
            second = first;
            first = cost[i][j];
        } else if (cost[i][j] <= second && cost[i][j] != first) {
            second = cost[i][j];
        }
    }
    return second;
}

// Recursive function for Branch and Bound
void TSPRec(int cost[V][V], int currBound, int currWeight, int level,
            int currPath[], bool visited[]) {
    if (level == V) {
        // Check for edge back to start
        if (cost[currPath[level-1]][currPath[0]] != 0) {
            int currRes = currWeight + cost[currPath[level-1]][currPath[0]];
            if (currRes < finalRes) {
                copyToFinal(currPath);
                finalRes = currRes;
            }
        }
        return;
    }

    for (int i = 0; i < V; i++) {
        if (cost[currPath[level-1]][i] != 0 && !visited[i]) {
            int temp = currBound;
            currWeight += cost[currPath[level-1]][i];

            // Different computation of currBound for level 1 and higher levels
            if (level == 1) {
                currBound -= ((firstMin(cost, currPath[level-1]) + firstMin(cost, i)) / 2);
            } else {
                currBound -= ((secondMin(cost, currPath[level-1]) + firstMin(cost, i)) / 2);
            }

            if (currBound + currWeight < finalRes) {
                currPath[level] = i;
                visited[i] = true;
                TSPRec(cost, currBound, currWeight, level + 1, currPath, visited);
            }

            // Backtrack
            currWeight -= cost[currPath[level-1]][i];
            currBound = temp;

            for (int j = 0; j < V; j++)
                visited[j] = false;
            for (int j = 0; j < level; j++)
                visited[currPath[j]] = true;
        }
    }
}

// Starter function
void TSP(int cost[V][V]) {
    int currPath[V+1];
    bool visited[V];

    for (int i = 0; i < V; i++) {
        currPath[i] = -1;
        visited[i] = false;
    }

    // Compute initial bound
    int currBound = 0;
    for (int i = 0; i < V; i++)
        currBound += (firstMin(cost, i) + secondMin(cost, i));

    currBound = (currBound & 1) ? currBound/2 + 1 : currBound/2;

    visited[0] = true;
    currPath[0] = 0;

    TSPRec(cost, currBound, 0, 1, currPath, visited);
}

int main() {
    int cost[V][V] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    TSP(cost);

    printf("Minimum cost Hamiltonian Cycle: %d\n", finalRes);
    printf("Path: ");
    for (int i = 0; i <= V; i++) {
        printf("%d ", finalPath[i]);
    }

    return 0;
}
