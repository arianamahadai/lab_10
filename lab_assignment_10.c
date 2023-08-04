#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    int count;             // To store the count of occurrences
    struct TrieNode* next[26];  // Array to hold pointers to child nodes (a-z)
};

// Trie structure
struct Trie
{	
    struct TrieNode* root;
};

// Function to create a new Trie node
struct TrieNode* createTrieNode()
{
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    newNode->count = 0;
    for (int i = 0; i < 26; i++)
        newNode->next[i] = NULL;
    return newNode;
}

// Function to initialize a trie structure
struct Trie* createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    trie->root = createTrieNode();
    return trie;
}

// Function to insert a word into the trie
void insert(struct Trie* trie, char* word)
{
    struct TrieNode* node = trie->root;
    for (int i = 0; word[i]; i++)
    {
        int index = word[i] - 'a';
        if (node->next[index] == NULL)
            node->next[index] = createTrieNode();
        node = node->next[index];
    }
    node->count++; // Increment the count when the entire word is inserted
}

// Function to find the number of occurrences of a word in the trie
int numberOfOccurrences(struct Trie* trie, char* word)
{
    struct TrieNode* node = trie->root;
    for (int i = 0; word[i]; i++)
    {
        int index = word[i] - 'a';
        if (node->next[index] == NULL)
            return 0; // Word not found, return 0 occurrences
        node = node->next[index];
    }
    return node->count; // Return the count of occurrences
}

// Function to deallocate the trie structure
void deallocateTrieNode(struct TrieNode* node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < 26; i++)
        deallocateTrieNode(node->next[i]);

    free(node);
}

struct Trie* deallocateTrie(struct Trie* trie)
{
    deallocateTrieNode(trie->root);
    free(trie);
    return NULL;
}

// Function to read the dictionary and store words in pInWords array
int readDictionary(char* filename, char** pInWords)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Failed to open the dictionary file.\n");
        exit(1);
    }

    int numWords = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0'; // Remove the newline character

        pInWords[numWords] = strdup(buffer);
        if (pInWords[numWords] == NULL)
        {
            printf("Memory allocation failed for word: %s\n", buffer);
            exit(1);
        }

        printf("Read word: %s\n", pInWords[numWords]);
        numWords++;
    }

    fclose(fp);
    return numWords;
}

int main(void)
{
    char* inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    printf("Number of words in the dictionary: %d\n", numWords);
    
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    if (pTrie == NULL)
    {
        printf("Failed to create Trie.\n");
        return 1;
    }

    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
        printf("Trie deallocated successfully.\n");

    // Free the memory allocated for the words in the dictionary
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}