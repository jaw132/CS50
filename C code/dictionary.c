// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
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
const unsigned int N = 32063;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //convert word to lowercase
    int n = strlen(word);
    char *lower = malloc(n);
    for (int i = 0; i<n; i++){
        lower[i] = tolower(word[i]);
    }

    unsigned int checkhash = hash(lower);

    for(node *tmp = table[checkhash]; tmp != NULL; tmp = tmp->next){
        if (strcmp(tmp->word, lower) == 0){
            free(lower);
            return true;
        }
    }

    free(lower);


    return false;
}

// Hashes word to a number
// http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //initialise each node in array to NULL
    for (int i = 0; i < N; i++){
        table[i] = NULL;
    }


    //read each word in using \n
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return 1;
    }

    int index = 0;
    char dicWord[LENGTH + 1];

    // Spell-check each word in text
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {

        // Allow only alphabetical characters and apostrophes
        if (c != '\n')
        {
            //printf("Entered if\n");
            // Append character to word
            dicWord[index] = c;
            index++;
        }
        else{
            dicWord[index] = '\0';

            //printf("pre hash\n");
            unsigned int dictIndex = hash(dicWord);
            //printf("%i\n", dictIndex);
            //printf("post hash\n");

            node *n = malloc(sizeof(node));
            strcpy(n->word, dicWord);
            //printf("new word %s\n", n->word);
            n->next = NULL;

            //printf("pointers\n");

            if (table[dictIndex] == NULL){
                table[dictIndex] = n;
            }
            else{
                node *tmp = table[dictIndex];
                while(tmp->next){
                    tmp = tmp->next;
                }
                tmp->next = n;
            }
            //printf("find NULL\n");

            //printf("inputted word %s\n", table[dictIndex]->word);

            index = 0;


        }

    }


    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int words = 0;

    for (int i = 0; i < N; i++){
        if (table[i] != NULL){
            words++;
            node *ptr = table[i];
            while(ptr->next){
                words++;
                ptr=ptr->next;
            }
        }
    }
    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++){
        while(table[i] != NULL){
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }


    if (table[8622] == NULL){
        return true;
    }

    return false;
}
