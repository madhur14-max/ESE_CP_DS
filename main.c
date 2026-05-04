#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "suffix.h"
#include "trie.h"
#include "pq.h"
#include "huffman.h"
#include "segment_tree.h"

// ================= LOAD DATA =================
void loadDataFromFile(TrieNode* root, SuffixIndex* si, char* filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char word[100];
    int freq;

    while (fscanf(fp, "%s %d", word, &freq) != EOF) {
        insertWord(root, word, freq);
        addWordToSuffixIndex(si, word, freq);
    }

    fclose(fp);
    printf("Data loaded successfully from %s!\n", filename);
}

// ================= SAVE DATA =================
void saveTrieToFileHelper(TrieNode* node, FILE* fp, char* buffer, int depth) {
    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        fprintf(fp, "%s %d\n", buffer, node->frequency);
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            buffer[depth] = i + 'a';
            saveTrieToFileHelper(node->children[i], fp, buffer, depth + 1);
        }
    }
}

void saveDataToFile(TrieNode* root, char* filename) {
    FILE* fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error saving file!\n");
        return;
    }

    char buffer[100];
    saveTrieToFileHelper(root, fp, buffer, 0);

    fclose(fp);
}

// ================= SAVE HISTORY =================
void saveHistory(char* query) {
    FILE* fp = fopen("history.txt", "a"); // append mode

    if (fp == NULL) {
        printf("Error opening history file!\n");
        return;
    }

    fprintf(fp, "%s\n", query);
    fclose(fp);
}

// ================= SHOW HISTORY =================
void showHistory() {
    FILE* fp = fopen("history.txt", "r");

    if (fp == NULL) {
        printf("No history found.\n");
        return;
    }

    char line[100];
    int count = 1;

    printf("\nSearch History:\n");

    while (fgets(line, sizeof(line), fp)) {
        printf("%d. %s", count++, line);
    }

    fclose(fp);
}

// ================= REBUILD SUFFIX =================
void rebuildSuffixIndex(TrieNode* node, SuffixIndex* si, char* buffer, int depth) {
    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        addWordToSuffixIndex(si, buffer, node->frequency);
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            buffer[depth] = i + 'a';
            rebuildSuffixIndex(node->children[i], si, buffer, depth + 1);
        }
    }
}

// ================= ADD NEW WORD =================
void addNewWord(TrieNode* root, SuffixIndex* si, char* filename) {
    char word[100];

    printf("Enter new word: ");
    scanf("%s", word);

    // frequency starts from 0
    insertWord(root, word, 0);
    addWordToSuffixIndex(si, word, 0);

    saveDataToFile(root, filename);

    printf("Word added successfully with frequency 0!\n");
}

// ================= MAIN =================
int main() {
    TrieNode* root = createNode();
    PriorityQueue pq;
    initPQ(&pq);

    SuffixIndex si;
    initSuffixIndex(&si);

    char filename[] = "data.txt";

    loadDataFromFile(root, &si, filename);

    int choice;
    char input[100];

    while (1) {
        printf("\n===== SMART SEARCH ENGINE =====\n");
        printf("1. Search Prefix\n");
        printf("2. Search Substring\n");
        printf("3. Show History\n");
        printf("4. Add New Word\n");
        printf("5. Exit\n");
        printf("6. Compress Database (Huffman)\n");
        printf("7. Range Search by Frequency\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // ===== PREFIX =====
        if (choice == 1) {
            initPQ(&pq);

            printf("Enter prefix: ");
            scanf("%s", input);

            saveHistory(input);

            clock_t start = clock();

            searchPrefix(root, input, &pq);

            clock_t end = clock();

            printf("\nTop Results:\n");

            int k = 3, count = 0;

            while (pq.size > 0 && count < k) {
                PQNode top = extractMax(&pq);
                printf("%s (freq: %d)\n", top.word, top.frequency);
                count++;
            }

            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Time taken: %f seconds\n", time_taken);

            saveDataToFile(root, filename);

            initSuffixIndex(&si);
            char buffer[100];
            rebuildSuffixIndex(root, &si, buffer, 0);

            printf("\nVisual Trie:\n");
            showTrieForPrefix(root, input);
        }

        // ===== SUBSTRING =====
        else if (choice == 2) {
            initPQ(&pq);

            printf("Enter substring: ");
            scanf("%s", input);

            saveHistory(input);

            clock_t start = clock();

            searchSubstring(&si, input, &pq, root);

            clock_t end = clock();

            printf("\nTop Results:\n");

            int k = 3, count = 0;

            while (pq.size > 0 && count < k) {
                PQNode top = extractMax(&pq);
                printf("%s (freq: %d)\n", top.word, top.frequency);
                count++;
            }

            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Time taken: %f seconds\n", time_taken);

            saveDataToFile(root, filename);

            initSuffixIndex(&si);
            char buffer[100];
            rebuildSuffixIndex(root, &si, buffer, 0);
        }

        else if (choice == 3) {
            showHistory();
        }

        else if (choice == 4) {
            addNewWord(root, &si, filename);
        }

        else if (choice == 6) {
            printf("\n--- Huffman Compression ---\n");
            compressDatabase(filename);
        }

        else if (choice == 7) {
            printf("\n--- Frequency Range Search ---\n");
            int min_f, max_f;
            printf("Enter minimum frequency: ");
            scanf("%d", &min_f);
            printf("Enter maximum frequency: ");
            scanf("%d", &max_f);
            
            saveDataToFile(root, filename); 
            
            SegmentTree st;
            initSegmentTree(&st, filename);
            rangeSearchByFrequency(&st, min_f, max_f);
            freeSegmentTree(&st);
        }

        else {
            break;
        }
    }

    return 0;
}
