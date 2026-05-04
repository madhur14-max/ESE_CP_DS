#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

// Create a new Huffman tree node
HuffNode* newHuffNode(char data, unsigned freq) {
    HuffNode* node = (HuffNode*)malloc(sizeof(HuffNode));
    node->left = node->right = NULL;
    node->data = data;
    node->freq = freq;
    return node;
}

// Create a Min-Heap of given capacity
MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffNode**)malloc(minHeap->capacity * sizeof(HuffNode*));
    return minHeap;
}

// Swap two min heap nodes
void swapHuffNode(HuffNode** a, HuffNode** b) {
    HuffNode* t = *a;
    *a = *b;
    *b = t;
}

// Standard Min-Heapify function
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapHuffNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if size of heap is 1
int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Extract minimum value node from heap
HuffNode* extractMin(MinHeap* minHeap) {
    HuffNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node into Min-Heap
void insertMinHeap(MinHeap* minHeap, HuffNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Standard function to build min heap
void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// The main function that builds Huffman tree
HuffNode* buildHuffmanTree(char data[], int freq[], int size) {
    HuffNode *left, *right, *top;
    MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newHuffNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // '$' is a special value for internal nodes, not used
        top = newHuffNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Global array to store huffman codes for 256 characters
char huffmanCodes[256][256];

// Function to store the huffman codes in the global array
void storeCodes(HuffNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1);
    }
    if (!(root->left) && !(root->right)) {
        char code[256];
        for (int i = 0; i < top; i++) {
            code[i] = arr[i] ? '1' : '0';
        }
        code[top] = '\0';
        huffmanCodes[(unsigned char)root->data][0] = '\0';
        strcpy(huffmanCodes[(unsigned char)root->data], code);
    }
}

// Recursively free the huffman tree
void freeHuffmanTree(HuffNode* root) {
    if (!root) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

// Compress the given dataset file and save as simulated binary
void compressDatabase(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening %s\n", filename);
        return;
    }

    // 1. Calculate global character frequency
    unsigned int freq[256] = {0};
    int c;
    int original_char_count = 0;
    while ((c = fgetc(fp)) != EOF) {
        freq[c]++;
        original_char_count++;
    }
    fclose(fp);

    if (original_char_count == 0) {
        printf("Database is empty, nothing to compress.\n");
        return;
    }

    int unique_chars = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) unique_chars++;
    }

    char *dataArr = (char *)malloc(unique_chars * sizeof(char));
    int *freqArr = (int *)malloc(unique_chars * sizeof(int));

    int idx = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            dataArr[idx] = (char)i;
            freqArr[idx] = freq[i];
            idx++;
        }
    }

    for (int i = 0; i < 256; i++) {
        huffmanCodes[i][0] = '\0';
    }

    // 2. Build Huffman Tree
    HuffNode* root = buildHuffmanTree(dataArr, freqArr, unique_chars);

    // 3. Generate prefix-free binary codes
    int arr[256], top = 0;
    storeCodes(root, arr, top);

    // 4. Encode the file
    fp = fopen(filename, "r");
    FILE *outFp = fopen("compressed.bin", "w"); // Simulated binary file
    int binary_bit_count = 0;

    while ((c = fgetc(fp)) != EOF) {
        fprintf(outFp, "%c:%s\n", c, huffmanCodes[c]);
        binary_bit_count += strlen(huffmanCodes[c]);
    }

    fclose(fp);
    fclose(outFp);
    freeHuffmanTree(root);
    free(dataArr);
    free(freqArr);

    // 5. Print results
    int original_bits = original_char_count * 8;
    printf("\nDatabase compressed successfully to 'compressed.bin'!\n");
    printf("Original Size: %d characters (%d bits)\n", original_char_count, original_bits);
    printf("Compressed Size: %d bits\n", binary_bit_count);
    
    if (original_bits > 0) {
        double saved = 100.0 * (original_bits - binary_bit_count) / original_bits;
        printf("Space Saved: %.2f%%\n", saved);
    }
}
