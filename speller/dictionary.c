// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <cs50.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 5000;

int word_count = 0;
// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word to obtain a hash value
    int hash_index = hash(word);

    // create a cursor node and point it to the head
    node *cursor = table[hash_index];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // find the sum of the ASCII values and multiply the sum by the first char
    int n = strlen(word);
    unsigned int sum = 0;
    unsigned int hash = 0;
    for (int i = 0; i < n; i++)
    {
        if (isalpha(word[i]))
        {
            sum += tolower(word[i]);
        }
        else
        {
            sum += word[i];
        }

    }
    hash = (sum * tolower(word[0])) % N;  //modulo the hash vaule so it stays in the size of the hash table array

    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *infile = fopen(dictionary, "r");
    if (!infile)
    {
        printf("Cannot open dictionary file.\n");
        return 1;
    }
    // read strings from file one at a time
    char buffer[LENGTH + 1]; // create an char array to read into
    while (fscanf(infile, "%s", buffer) != EOF) //read into buffer array until end of file
    {
        // allocate memory for a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Unable to allocate memory.\n");
            return 1;
        }

        // copy buffer into node
        strcpy(n->word, buffer);

        // hash word to obtain a hash value
        int hash_index = hash(n->word);

        // insert node into hash table at that location
        if (table[hash_index] == NULL)  // if the hash table at the index is empty
        {
            n->next = NULL;  // set the node pointer to NULL
            table[hash_index] = n;  //point the table header to the node
        }
        else  //if the hash table already has a linked list existing at the index location
        {
            n->next = table[hash_index];  //point the node to the table header (at the start of the linked list)
            table[hash_index] = n;  //change the table header to the node
        }
        word_count++;
    }

    fclose(infile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{

    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++) // iterate over hash table to see if any table header are not NULL
    {
        node *cursor = table[i];  //create a cursor node and point it to the table header

        while (cursor != NULL)  // loop thru the list when cursor not pointing to NULL
        {
            node *temp = cursor; //create a temp pointer pointing to cursor
            cursor = cursor->next; //move cursor to the next pointer in the linked list
            free(temp);  //free temp
        }
    }

    return true;
}
