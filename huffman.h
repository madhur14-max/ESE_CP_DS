#ifndef HUFFMAN_H
#define HUFFMAN_H

// Min-Heap node for Huffman Tree
typedef struct HuffNode {
    char data;
    unsigned freq;
    struct HuffNode *left, *right;
} HuffNode;

// Min-Heap structure
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffNode **array;
} MinHeap;

// Function to compress the database file
void compressDatabase(const char *filename);

#endif // HUFFMAN_H
