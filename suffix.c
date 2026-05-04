#include <stdio.h>
#include <string.h>
#include "suffix.h"
#include "pq.h"

// Initialize
void initSuffixIndex(SuffixIndex* si) {
    si->size = 0;
}

// Add suffixes
void addWordToSuffixIndex(SuffixIndex* si, char* word, int freq) {
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        strcpy(si->arr[si->size].suffix, word + i);
        strcpy(si->arr[si->size].original, word);
        si->arr[si->size].frequency = freq;
        si->size++;
    }
}

// Check duplicate
int isDuplicate(char results[][100], int count, char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(results[i], word) == 0)
            return 1;
    }
    return 0;
}

// Search substring
void searchSubstring(SuffixIndex* si, char* pattern, PriorityQueue* pq, TrieNode* root) {
    char uniqueResults[100][100];
    int count = 0;

    for (int i = 0; i < si->size; i++) {
        if (strncmp(si->arr[i].suffix, pattern, strlen(pattern)) == 0) {

            if (!isDuplicate(uniqueResults, count, si->arr[i].original)) {

                // update frequency in Trie
                updateFrequency(root, si->arr[i].original);

                // store unique result
                strcpy(uniqueResults[count], si->arr[i].original);

                // insert UPDATED frequency into PQ
                insertPQ(pq, si->arr[i].original, si->arr[i].frequency + 1);

                count++;
            }
        }
    }

    if (count == 0) {
        printf("No matches found.\n");
    }
}
