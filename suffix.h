#ifndef SUFFIX_H
#define SUFFIX_H

#include "pq.h"
#include "trie.h"

#define MAX_WORDS 1000
#define MAX_LEN 100

typedef struct {
    char suffix[MAX_LEN];
    char original[MAX_LEN];
    int frequency;
} SuffixNode;

typedef struct {
    SuffixNode arr[MAX_WORDS];
    int size;
} SuffixIndex;

void initSuffixIndex(SuffixIndex* si);
void addWordToSuffixIndex(SuffixIndex* si, char* word, int freq);
void searchSubstring(SuffixIndex* si, char* pattern, PriorityQueue* pq, TrieNode* root);

#endif
