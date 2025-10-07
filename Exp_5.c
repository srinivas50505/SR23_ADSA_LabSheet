#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct Edge {
    int u, v;
    struct Edge* next;
} Edge;

Edge* edgeStack = NULL;

void pushEdge(int u, int v) {
    Edge* e = (Edge*) malloc(sizeof(Edge));
    e->u = u; e->v = v;
    e->next = edgeStack;
    edgeStack = e;
}

Edge* popEdge() {
    if (!edgeStack) return NULL;
    Edge* e = edgeStack;
    edgeStack = edgeStack->next;
    return e;
}

int timeCounter;

void printBicomponent(Edge* e) {
    printf("Biconnected component: ");
    while (e) {
        printf("(%d, %d) ", e->u, e->v);
        Edge* temp = e;
        e = e->next;
        free(temp);
    }
    printf("\n");
}

void BCCUtil(int u, int parent, int disc[], int low[], int visited[], int graph[MAX][MAX], int vertices) {
    static int stackIndex = 0;
    static int stack[MAX * MAX][2];
    static int top = -1;
    
    static int time = 0;
    disc[u] = low[u] = ++time;
    visited[u] = 1;

    for (int v = 0; v < vertices; v++) {
        if (graph[u][v]) {
            if (disc[v] == -1) {
                pushEdge(u, v);
                BCCUtil(v, u, disc, low, visited, graph, vertices);
                low[u] = (low[u] < low[v]) ? low[u] : low[v];

                // If low[v]>=disc[u], u is articulation point, pop edges for component
                if (low[v] >= disc[u]) {
                    Edge* component = NULL;
                    Edge* e;
                    do {
                        e = popEdge();
                        e->next = component;
                        component = e;
                    } while(!(e->u == u && e->v == v));
                    printBicomponent(component);
                }
            } else if (v != parent && disc[v] < disc[u]) {
                low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
                pushEdge(u, v);
            }
        }
    }
}

void BCC(int graph[MAX][MAX], int vertices) {
    int disc[MAX], low[MAX], visited[MAX] = {0};
    for (int i = 0; i < vertices; i++) {
        disc[i] = -1;
        low[i] = -1;
    }
    for (int i = 0; i < vertices; i++) {
        if (disc[i] == -1)
            BCCUtil(i, -1, disc, low, visited, graph, vertices);
    }
}

int main() {
    int vertices = 5;
    int graph[MAX][MAX] = {0};
    // Example graph edges (undirected)
    graph[0][1] = graph[1][0] = 1;
    graph[1][2] = graph[2][1] = 1;
    graph[2][0] = graph[0][2] = 1;
    graph[1][3] = graph[3][1] = 1;
    graph[3][4] = graph[4][3] = 1;

    BCC(graph, vertices);

    return 0;
}
