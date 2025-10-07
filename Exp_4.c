#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

// Queue structure for BFT
typedef struct {
    int items[MAX];
    int front, rear;
} Queue;

void initQueue(Queue *q) {
    q->front = -1; q->rear = -1;
}

int isEmpty(Queue *q) {
    return q->front == -1;
}

void enqueue(Queue *q, int val) {
    if (q->rear == MAX - 1) return;
    if (q->front == -1) q->front = 0;
    q->items[++q->rear] = val;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) return -1;
    int val = q->items[q->front];
    if (q->front == q->rear) q->front = q->rear = -1;
    else q->front++;
    return val;
}

// GRAPH: Adjacency Matrix Part
void BFT_Matrix(int graph[MAX][MAX], int vertices, int start) {
    bool visited[MAX] = {false};
    Queue q;
    initQueue(&q);
    visited[start] = true;
    enqueue(&q, start);
    printf("BFT (Adj Mat): ");
    while (!isEmpty(&q)) {
        int u = dequeue(&q);
        printf("%d ", u);
        for (int v = 0; v < vertices; v++) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = true;
                enqueue(&q, v);
            }
        }
    }
    printf("\n");
}

void DFT_MatrixUtil(int graph[MAX][MAX], int vertices, int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);
    for (int i = 0; i < vertices; i++) {
        if (graph[v][i] && !visited[i])
            DFT_MatrixUtil(graph, vertices, i, visited);
    }
}

void DFT_Matrix(int graph[MAX][MAX], int vertices, int start) {
    bool visited[MAX] = {false};
    printf("DFT (Adj Mat): ");
    DFT_MatrixUtil(graph, vertices, start, visited);
    printf("\n");
}

// GRAPH: Adjacency List Part
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node* adjLists[MAX];
} Graph;

Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    // Add edge src->dest
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    // For undirected graph add dest->src also
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void BFT_List(Graph* graph, int start) {
    bool visited[MAX] = {false};
    Queue q;
    initQueue(&q);
    visited[start] = true;
    enqueue(&q, start);
    printf("BFT (Adj List): ");
    while (!isEmpty(&q)) {
        int u = dequeue(&q);
        printf("%d ", u);
        Node* temp = graph->adjLists[u];
        while (temp) {
            int v = temp->vertex;
            if (!visited[v]) {
                visited[v] = true;
                enqueue(&q, v);
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

void DFT_ListUtil(Graph* graph, int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);
    Node* temp = graph->adjLists[v];
    while (temp) {
        int adjV = temp->vertex;
        if (!visited[adjV])
            DFT_ListUtil(graph, adjV, visited);
        temp = temp->next;
    }
}

void DFT_List(Graph* graph, int start) {
    bool visited[MAX] = {false};
    printf("DFT (Adj List): ");
    DFT_ListUtil(graph, start, visited);
    printf("\n");
}

// Main function to demonstrate
int main() {
    int vertices = 6;

    // Adjacency Matrix
    int graphMat[MAX][MAX] = {0};
    // Adding edges (undirected)
    graphMat[0][1] = 1;
    graphMat[1][0] = 1;
    graphMat[0][2] = 1;
    graphMat[2][0] = 1;
    graphMat[1][3] = 1;
    graphMat[3][1] = 1;
    graphMat[1][4] = 1;
    graphMat[4][1] = 1;
    graphMat[2][5] = 1;
    graphMat[5][2] = 1;

    int startVertex = 0;

    BFT_Matrix(graphMat, vertices, startVertex);
    DFT_Matrix(graphMat, vertices, startVertex);

    // Adjacency List
    Graph* graphList = createGraph(vertices);
    addEdge(graphList, 0, 1);
    addEdge(graphList, 0, 2);
    addEdge(graphList, 1, 3);
    addEdge(graphList, 1, 4);
    addEdge(graphList, 2, 5);

    BFT_List(graphList, startVertex);
    DFT_List(graphList, startVertex);

    return 0;
}
