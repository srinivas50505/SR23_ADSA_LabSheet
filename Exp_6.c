#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Quick sort partition
int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++; 
            int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
        }
    }
    int temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;
    return i + 1;
}

// Quick sort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi -1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge two sorted halves
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    int* L = malloc(n1 * sizeof(int));
    int* R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

// Merge sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left)/2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

// Utility to generate arrays for test cases
void generateArray(int arr[], int n, int caseType) {
    if (caseType == 0) { // Average case - random
        for (int i = 0; i < n; i++)
            arr[i] = rand() % (n*10);
    } else if (caseType == 1) { // Best case - sorted ascending
        for (int i = 0; i < n; i++)
            arr[i] = i;
    } else { // Worst case - sorted descending
        for (int i = 0; i < n; i++)
            arr[i] = n - i;
    }
}

int main() {
    srand(time(NULL));
    int n[] = {1000, 5000, 10000}; // Change sizes as needed
    char* caseNames[] = {"Average", "Best", "Worst"};

    printf("Size,Case,QuickSort(ms),MergeSort(ms)\n");

    for (int sizeIdx = 0; sizeIdx < 3; sizeIdx++) {
        int size = n[sizeIdx];
        int* arr1 = malloc(size * sizeof(int));
        int* arr2 = malloc(size * sizeof(int));

        for (int caseIdx = 0; caseIdx < 3; caseIdx++) {
            generateArray(arr1, size, caseIdx);
            for (int i = 0; i < size; i++) arr2[i] = arr1[i];

            clock_t start = clock();
            quickSort(arr1, 0, size -1);
            clock_t end = clock();
            double quickTime = (double)(end - start) / CLOCKS_PER_SEC * 1000;

            start = clock();
            mergeSort(arr2, 0, size -1);
            end = clock();
            double mergeTime = (double)(end - start) / CLOCKS_PER_SEC * 1000;

            printf("%d,%s,%.2f,%.2f\n", size, caseNames[caseIdx], quickTime, mergeTime);
        }

        free(arr1);
        free(arr2);
    }
    return 0;
}
