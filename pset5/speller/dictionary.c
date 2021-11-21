// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65535;

// Hash table
node *table[N];

// Number of loaded words
int loaded;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char text[strlen(word) + 1];
    strcpy(text,word);
    text[strlen(word)] = '\0';

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        text[i] = tolower(text[i]);
    }

    int index = hash(text);
    if(table[index] == NULL)
    {
        return false;
    }
    else{
        node *tmp = table[index];
        while(tmp != NULL)
        {
            if (strcmp(text, tmp->word) == 0)
            {
                return true;
            }

            tmp = tmp->next;
        }
    }


    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int hashnum = 0, len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        hashnum += len + word[i]*i;
    }

    return hashnum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        return false;
    }

    char buffer[LENGTH + 2];

    //sets library
    loaded = 0;

    while (fgets(buffer, sizeof(buffer), dict))
    {
        buffer[strlen(buffer) - 1] = '\0';

        node *n = calloc(1, sizeof(node));
        strncpy(n->word, buffer, strlen(buffer));
        n->next = NULL;

        int index = hash(buffer);

        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            node *temp = table[index];

            while (temp->next != NULL)
            {
                temp = temp->next;
            }

            temp->next = n;
        }
        loaded++;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return loaded;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int unloaded = 0;
    for (int i = 0; i < N; i++)
    {
        if (table[i])
        {
            node *temp = table[i];

            while (temp->next != NULL)
            {
                temp = temp->next;
                free(table[i]);
                unloaded++;
                table[i] = temp;
            }

            free(table[i]);
            unloaded++;


        }
    }

    if (unloaded == loaded)
    {
        return true;
    }

    return false;
}
