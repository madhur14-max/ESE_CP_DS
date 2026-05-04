#include <stdio.h>
#include <string.h>
#include "pq.h"

void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}

void swap(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

// Insert into max heap
void insertPQ(PriorityQueue* pq, char* word, int freq) {
    pq->arr[pq->size].frequency = freq;
    strcpy(pq->arr[pq->size].word, word);

    heapifyUp(pq, pq->size);
    pq->size++;
}

// Move up
void heapifyUp(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (pq->arr[parent].frequency < pq->arr[index].frequency) {
            swap(&pq->arr[parent], &pq->arr[index]);
            index = parent;
        } else {
            break;
        }
    }
}

// Move down
void heapifyDown(PriorityQueue* pq, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->arr[left].frequency > pq->arr[largest].frequency)
        largest = left;

    if (right < pq->size && pq->arr[right].frequency > pq->arr[largest].frequency)
        largest = right;

    if (largest != index) {
        swap(&pq->arr[index], &pq->arr[largest]);
        heapifyDown(pq, largest);
    }
}

// Remove max
PQNode extractMax(PriorityQueue* pq) {
    PQNode max = pq->arr[0];
    pq->arr[0] = pq->arr[pq->size - 1];
    pq->size--;

    heapifyDown(pq, 0);

    return max;
}

// Helper to print heap recursively like a tree
void printHeapVisualHelper(PriorityQueue* pq, int index, int space) {
    if (index >= pq->size) return;

    // Increase distance between levels
    space += 7;

    // Process right child first
    printHeapVisualHelper(pq, 2 * index + 2, space);

    // Print current node after space count
    printf("\n");
    for (int i = 7; i < space; i++)
        printf(" ");
    printf("%s(%d)\n", pq->arr[index].word, pq->arr[index].frequency);

    // Process left child
    printHeapVisualHelper(pq, 2 * index + 1, space);
}

// Show priority queue structure visually
void showPQVisual(PriorityQueue* pq) {
    printf("\n--- Priority Queue (Max-Heap) Visual ---\n");
    if (pq->size == 0) {
        printf("Empty Priority Queue.\n");
    } else {
        printHeapVisualHelper(pq, 0, 0);
        printf("\n");
    }
    printf("----------------------------------------\n");
}