// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#include "dictionary.h"

#define HASHTABLE_SIZE 1000

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//insert hashtag prototype so my shit works
unsigned int hash(const char *word);

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table, which is an array of node pointers
node *table[HASHTABLE_SIZE];

//create a counter for the number of words, which is modified during the load function
int counter = 0;




// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // convert word to lowercase

    int n = strlen(word);
    char copy[n + 1];

    //add null character to end of word
    copy[n] = '\0';

    //loop to go through and make copy lowercase
    for (int i = 0; i<n; i++)
    {
        copy[i] = tolower(word[i]);
    }

    //pass the lowercase word through the hash function
    int index = hash(copy) % HASHTABLE_SIZE;

    //create node to pass through the hash table
    node *head = table[index];

    //make sure it is not NULL
    if (head != NULL)
    {
       //create cursor and point it to the head
       node *cursor = head;

       //traverse the list
       while (cursor != NULL)
       {
           if (strcmp(copy, cursor->word) == 0)
           {
               //return true because there is a match in the dictionary
               return true;
           }

           else
           {
               //move cursor to next node in list
               cursor = cursor->next;
           }
       }

    }

    return false;
}




// Hashes word to a number (function credit: Dan Bernstein on stack overflow)

unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}




// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open the dictionary for reading
    FILE *dicfile = fopen(dictionary, "r");

    //check to be sure dictionary exists
    if(dictionary == NULL)
    {
        printf("stderr: could not open dictionary");
        return false;
    }

    //create a buffer, an array of characters where the read words can be temporarily stored. LENGTH (constant value of 45) plus 1 for the NULL terminator
    char buffer[LENGTH + 1];

    //set n for the fgets() function, plus two so that the null terminator is included
    int n = LENGTH + 2;

    //run a while loop to pull out strings and save them into our buffer. Loop stops when it hits the end of the file (when fgets is not NULL)
    while (fgets(buffer, n, dicfile) != NULL)
    {
        //add a null character to the end of the word
        buffer[strlen(buffer) - 1] = '\0';

        //run the word through the hash function
        int index = hash(buffer) % HASHTABLE_SIZE;

        //create a temporary node
        node *tmp = malloc(sizeof(node));

        //check to be sure malloc is not null
        if(tmp == NULL)
        {
            fclose(dicfile);
            return false;
        }

        //move node to next in list
        strcpy(tmp->word, buffer);
        tmp->next = table[index];

        table[index] = tmp;

        //counter to keep track of words inserted
        counter++;

    }

    //close file
    fclose(dicfile);

    return true;
}




// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //the counting is done in the load function, so this just needs to return the number on the counter
    return counter;
}





// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //loop through the nodes in order to delete them
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
       //check for a note at index i
       node *cursor = table[i];
       while (cursor != NULL)
       {
           //create a temporary node
           node *tmp = cursor;
           cursor = cursor->next;

           //free the current node
           free(tmp);
       }
    }
    return false;
}
