// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 75011;

// Hash table
node *table[N];

// Check recursive helper function
bool check_helper(const char *word, node *position)
{
    if (strcmp(word, position->word) == 0)
    {
        return true;
    }
    else
    {
        if (position->next != NULL)
        {
            return check_helper(word, position->next);
        }
    }
    return false;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Turn word to lower_case
    char new_word[LENGTH + 1];
    int word_length = strlen(word);
    for (int i = 0; i < word_length; i++)
    {
        new_word[i] = (char) tolower(word[i]);
    }
    // Add null terminating character (!!!!), without it garbage from previous words is added at the end
    // *if they are longer then that current word 
    new_word[strlen(word)] = '\0';

    int find_index = hash(new_word);
    // Compare with all words in position returned by hash()
    if (table[find_index] == NULL)
    {
        // If there is no such word in dictionary
        return false;
    }
    else
    {
        return check_helper(new_word, table[find_index]);
    }   
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 by Dan Bernstein http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return (hash % N);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Read file
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }

    // Store word
    char temp[LENGTH + 1];
    // int number_of_collisions = 0;
    // int number_of_original_indices = 0;

    // Loop until end of file
    while (fscanf(f, "%s", temp) != EOF)
    {
        // Create new node 
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(f);
            return false;
        }
        else
        {
            // Copy temp to new_node->word
            strcpy(new_node->word, temp);
            new_node->next = NULL;
            
            // Find index
            int bucket = hash(new_node->word);
            
            // Check for collision
            if (table[bucket] == NULL)
            {
                // number_of_original_indices++;
                table[bucket] = new_node;
            }
            else
            {
                node *node_temp = table[bucket];
                
                // Find end of list
                while (node_temp->next != NULL)
                {
                    node_temp = node_temp->next;
                }

                // Insert at the end of the list
                node_temp->next = new_node;
                // number_of_collisions++;
            }
        }   
    }
    // for (int j = 0; j < 1; j++)
    // {
    //     if (table[j] == NULL)
    //     {
    //         printf("BUCKET[%i] = NULL\n", j);
    //     }
    //     else
    //     {
    //         node *temporary = table[j];
    //         while (temporary->next != NULL)
    //         {
    //             printf("BUCKET[%i] = %s\n", j, temporary->word);
    //             temporary = temporary->next;
    //         }
    //     }

    // }
    // printf("Number of collisions: %i\n", number_of_collisions);
    // printf("Number of original indices: %i\n", number_of_original_indices);
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // It would be easier to just create global variable and count words that are being stored in load()
    // But since it's part of exercise I presume it's more of a check if load() stored them correctly?
    int words_loaded = 0;
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            // Unused bucket
        }
        else
        {
            node *temporary = table[i];
            while (temporary != NULL)
            {
                words_loaded++;
                temporary = temporary->next;
            }
        }

    }
    return words_loaded;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            // Unused bucket
        }
        else
        {   
            // Point to the node that is supposed to be unloaded
            node *temporary = table[i];
            while (temporary != NULL)
            {
                // Make bucket point to next node in a linked list if it exists
                table[i] = (temporary->next != NULL) ? temporary->next : NULL;
                // Free memory
                free(temporary);
                // Make temporary point to the next element
                temporary = table[i];
            }
        }

    }
    return true;
}
