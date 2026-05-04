#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

// Structure to hold word and its frequency
typedef struct WordFreq {
    char word[100];
    int freq;
} WordFreq;

// Segment Tree Node
typedef struct SegNode {
    int min_freq;
    int max_freq;
} SegNode;

// Segment Tree Structure
typedef struct SegmentTree {
    WordFreq *words;
    SegNode *tree;
    int n;
} SegmentTree;

// Functions
void initSegmentTree(SegmentTree *st, const char *filename);
void rangeSearchByFrequency(SegmentTree *st, int min_f, int max_f);
void freeSegmentTree(SegmentTree *st);

#endif // SEGMENT_TREE_H
