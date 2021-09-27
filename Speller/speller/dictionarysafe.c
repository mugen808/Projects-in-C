// Implements a dictionary's functionality
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int key = hash(word);
    node *temp_word = malloc(sizeof(node));
    temp_word = table[key];
    if (temp_word == NULL)
    {
        return false;
    }

    while (true)
    {
        if (strcasecmp(temp_word->word, word) == 0)
        {
            return true;
            break;
        }
        if (temp_word->next == NULL)
        {
            return false;
            break;
        }

        temp_word = temp_word->next;


    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // assign a number to the first char of buffer from 0-25
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Linked and hash list inspired by maxib7 on StackOverflow (https://stackoverflow.com/questions/31930046/what-is-a-hash-table-and-how-do-you-make-it-in-c/31930047#31930047)

    // The scanned word is stored temporarily
    char temp_word[46];
    // Where to store the new read word

    // Open files and checks if it loads correctly
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }


    // Scan the file until EOF
    while (fscanf(file, "%s", temp_word) != EOF)
    {
        node *new_word = malloc(sizeof(node));
        if (new_word == NULL)
        {
            return false;
        }
        int key = hash(temp_word);
        strcpy(new_word->word, temp_word);
        new_word->next = NULL;
        if (table[key] == NULL)
        {
            table[key] = new_word;
        }
        else
        {
            node* previous_word = table[key];

            while(true)
            {
                if (previous_word->next == NULL)
                {
                    previous_word->next = new_word;
                    break;
                }
                previous_word = previous_word->next;
            }
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int counter = 0;
    node* null_searcher = malloc(sizeof(node));

    for (int i = 0; i < 26; i++)
    {
        null_searcher = table[i];

        while (null_searcher != NULL)
            {
                counter++;
                null_searcher = null_searcher->next;
            }
    }
    if (counter > 0)
    {
        free(null_searcher);
        return counter;
    }
    else
    {
        free(null_searcher);
        return 0;
    }
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node* checker = NULL;

    for (int i = 0; i < 26; i++)
    {
        if (table[i] == NULL)
        {
            i++;
        }
        else
        {
            checker = table[i];
            while (checker != NULL)
            {
            node* temp = checker;
            checker = checker->next;
            free(temp);
            }
        }
    }

    return true;
}
