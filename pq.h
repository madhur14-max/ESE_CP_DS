
#ifndef PQ_H
#define PQ_H

#define MAX 1000

typedef struct {
    char word[100];
    int frequency;
} PQNode;

typedef struct {
    PQNode arr[MAX];
    int size;
} PriorityQueue;

// Functions
void initPQ(PriorityQueue* pq);
void insertPQ(PriorityQueue* pq, char* word, int freq);
void heapifyUp(PriorityQueue* pq, int index);
void heapifyDown(PriorityQueue* pq, int index);
PQNode extractMax(PriorityQueue* pq);
void showPQVisual(PriorityQueue* pq);

#endif