#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "segment_tree.h"

// Comparator for sorting WordFreq
int compareWordFreq(const void *a, const void *b) {
    WordFreq *w1 = (WordFreq *)a;
    WordFreq *w2 = (WordFreq *)b;
    return w1->freq - w2->freq;
}

// Build the Segment Tree recursively
void buildSegTree(SegmentTree *st, int node, int start, int end) {
    if (start == end) {
        st->tree[node].min_freq = st->words[start].freq;
        st->tree[node].max_freq = st->words[start].freq;
        return;
    }
    int mid = (start + end) / 2;
    buildSegTree(st, 2 * node, start, mid);
    buildSegTree(st, 2 * node + 1, mid + 1, end);
    
    // min_freq is min of left child, max_freq is max of right child (because it's sorted)
    st->tree[node].min_freq = st->tree[2 * node].min_freq;
    st->tree[node].max_freq = st->tree[2 * node + 1].max_freq;
}

// Initialize the Segment Tree from data file
void initSegmentTree(SegmentTree *st, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        st->n = 0;
        return;
    }

    // Count words
    int count = 0;
    char w[100];
    int f;
    while (fscanf(fp, "%s %d", w, &f) != EOF) {
        count++;
    }
    st->n = count;

    if (count == 0) {
        fclose(fp);
        return;
    }

    // Allocate memory
    st->words = (WordFreq *)malloc(count * sizeof(WordFreq));
    st->tree = (SegNode *)malloc(4 * count * sizeof(SegNode));

    // Read words
    rewind(fp);
    int i = 0;
    while (fscanf(fp, "%s %d", st->words[i].word, &st->words[i].freq) != EOF) {
        i++;
    }
    fclose(fp);

    // Sort words by frequency
    qsort(st->words, count, sizeof(WordFreq), compareWordFreq);

    // Build the tree over the sorted array
    buildSegTree(st, 1, 0, count - 1);
}

// Query the Segment Tree for the given frequency range
void querySegTree(SegmentTree *st, int node, int start, int end, int min_f, int max_f) {
    // If current node's range is completely outside query range
    if (st->tree[node].max_freq < min_f || st->tree[node].min_freq > max_f) {
        return;
    }
    
    // If current node's range is completely inside query range
    if (st->tree[node].min_freq >= min_f && st->tree[node].max_freq <= max_f) {
        for (int i = start; i <= end; i++) {
            printf("- %s (freq: %d)\n", st->words[i].word, st->words[i].freq);
        }
        return;
    }
    
    // Partially inside, recurse
    int mid = (start + end) / 2;
    querySegTree(st, 2 * node, start, mid, min_f, max_f);
    querySegTree(st, 2 * node + 1, mid + 1, end, min_f, max_f);
}

// Public function for range search
void rangeSearchByFrequency(SegmentTree *st, int min_f, int max_f) {
    if (st->n == 0) {
        printf("Database is empty.\n");
        return;
    }
    
    // Validation
    if (min_f > max_f) {
        printf("Invalid range! Min frequency should be <= Max frequency.\n");
        return;
    }
    
    printf("\nWords with frequency between %d and %d:\n", min_f, max_f);
    querySegTree(st, 1, 0, st->n - 1, min_f, max_f);
}

// Free allocated memory
void freeSegmentTree(SegmentTree *st) {
    if (st->words) free(st->words);
    if (st->tree) free(st->tree);
    st->n = 0;
}
