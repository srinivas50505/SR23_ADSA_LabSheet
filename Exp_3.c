#include <stdio.h>

// Swap helper
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b; *b = temp;
}

// Heapify for Max Heap
void maxHeapify(int arr[], int n, int i) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        maxHeapify(arr, n, largest);
    }
}

// Build Max Heap
void buildMaxHeap(int arr[], int n) {
    for (int i = n/2-1; i >= 0; i--)
        maxHeapify(arr, n, i);
}

// Heapify for Min Heap
void minHeapify(int arr[], int n, int i) {
    int smallest = i, l = 2*i+1, r = 2*i+2;
    if (l < n && arr[l] < arr[smallest]) smallest = l;
    if (r < n && arr[r] < arr[smallest]) smallest = r;
    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        minHeapify(arr, n, smallest);
    }
}

// Build Min Heap
void buildMinHeap(int arr[], int n) {
    for (int i = n/2-1; i >= 0; i--)
        minHeapify(arr, n, i);
}

// Delete any element from heap and restore property
void deleteFromMaxHeap(int arr[], int* n, int idx) {
    if (idx < 0 || idx >= *n) return;
    arr[idx] = arr[*n-1];
    (*n)--;
    maxHeapify(arr, *n, idx);
}

void deleteFromMinHeap(int arr[], int* n, int idx) {
    if (idx < 0 || idx >= *n) return;
    arr[idx] = arr[*n-1];
    (*n)--;
    minHeapify(arr, *n, idx);
}

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {15, 3, 17, 10, 84, 19, 6, 22, 9};
    int n = sizeof(arr)/sizeof(arr[0]);

    // MAX HEAP
    int maxHeapArr[100], maxHeapSize = n;
    for (int i = 0; i < n; i++) maxHeapArr[i] = arr[i];
    buildMaxHeap(maxHeapArr, maxHeapSize);
    printf("Max Heap: ");
    printArray(maxHeapArr, maxHeapSize);

    // Delete element at index 2 (3rd element)
    deleteFromMaxHeap(maxHeapArr, &maxHeapSize, 2);
    printf("Max Heap after deletion: ");
    printArray(maxHeapArr, maxHeapSize);

    // MIN HEAP
    int minHeapArr[100], minHeapSize = n;
    for (int i = 0; i < n; i++) minHeapArr[i] = arr[i];
    buildMinHeap(minHeapArr, minHeapSize);
    printf("Min Heap: ");
    printArray(minHeapArr, minHeapSize);

    // Delete element at index 2 (3rd element)
    deleteFromMinHeap(minHeapArr, &minHeapSize, 2);
    printf("Min Heap after deletion: ");
    printArray(minHeapArr, minHeapSize);

    return 0;
}
