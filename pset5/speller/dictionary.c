// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//create global variable for tracking # of words
int word_counter = 0;

// Number of buckets in hash table. Set to 14,000 arbitrarily. Dictionary containts 143,000 words approx.
const unsigned int N = 14000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    //create an array the length of word where a lowercase copy can be stored
    char lowercaseword[LENGTH + 1];

    //run loop to convert word to lowercase
    for (int i = 0; i < LENGTH + 1; i++)
    {
        lowercaseword[i] = tolower(word[i]);
    }

    //hash word to obtain index
    int check_index = hash(lowercaseword);

    //create cursor and set it to hash table index
    node *cursor = table[check_index];

    //create a while look to go through the linked list word by word until it reaches the end (NULL)
    while (cursor != NULL)
    {
        //if the cursor is the same as the input word then there is a match
        if(strcasecmp(cursor->word, lowercaseword) == 0)
        {
            return true;
        }
        //otherwise the cursor points to the next word in the list
        cursor = cursor->next;
    }

    free(cursor);
    return false;
}

// Hashes word to a number. CREDIT: https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(word); i<n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //set word length
    char scanned_word[LENGTH + 1];

    //open dictionary for reading
    FILE *dicpointer = fopen(dictionary, "r");

    //run a while loop to pull out a string at a time, until the EOF
    while (fscanf(dicpointer, "%s", scanned_word) != EOF)
    {
        //for every word pulled out a new node needs to be created. Create node and allocate memory
        node *new_node = malloc(sizeof(node));
        //check to be sure it is not NULL
        if (new_node == NULL)
        {
            printf("stderr: creating node to upload word");
            return false;
        }

        //copy word to new node
        strcpy(new_node->word, scanned_word);

        //run the word through the hash function to return index value
        int index = hash(new_node->word);

        //point the new node at the same pointer as the head
        new_node->next = table[index];

        //set the head to point to the new node
        table[index] = new_node;

        //add one to counter
        word_counter++;
    }

    //close the dictionary file opened previously
    fclose(dicpointer);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // simply returns word counter. Actual counting is done in the load function
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //create loop to go through index of hash table index. N is the number in the hashtable array
    for(int i = 0; i < N; i++)
    {
        //create a cursor to traverse the linked list. Point it to first node.
        node *cursor = table[i];

        //create a temp cursor and set it to point at the same thing as cursor
        node *temp = cursor;

        //set while loop to go through until end of list
        while(cursor != NULL)
        {
            //move cursor to next node
            cursor = cursor->next;

            //free temp cursor
            free(temp);

            //set temp to match the new thing that cursor is pointing to
            temp = cursor;
        }
        free(cursor);

    }
    return true;
}
