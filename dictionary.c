// Implements a dictionary's functionality

#include <stdbool.h>
#define _GNU_SOURCE // for rawmemchr - GNU extension
#include <sys/stat.h> // for stat struct
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memchr and strcmp
#include <stdint.h> // for murmur hash
#include <pthread.h> // todo
#include "dictionary.h"

typedef struct node
{
    char word[LENGTH];
    struct node *next;
    struct node *previous;
}
node;

typedef struct list
{
    int position[3];
    struct list *next;
}
collisionsList;

node *CreateNode(char *wordToAdd, int wordLength);
collisionsList *CreateCollisionsList(int indicie[3]);

node *AddNode(node *head, char *wordToAdd, int wordLength);
collisionsList *AddCollisionsList(collisionsList *head, int indicie[3]);

void deleteHashtable(node *start);

// for bitwise OR to lowercase
#define BITS 32

//Hash table 0 - 25 = A - Z ' =  26
node *tableOverFive[27][27][27] = {{{NULL}}};

//Hash table 1 - 26 = A - Z 27 =  ' No char=  0
bool tableUnderFive[28][28][28] = {{{NULL}}};

node *traversalNode;

collisionsList *currentCollisionsList;
collisionsList *previousCollisionsList;

//Array to store current words first 5 ASCII values
int currentWordFirst5[3];
int currentWordLength;
int numberOfWords = 0;
int collisionsNumber = 0;
int wordsLongerThan5 = 0;
float wordLengthTotal = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    currentWordLength = strlen(word);
    char temp[currentWordLength];
    for (int i = 0; i < currentWordLength; i++)
    {
        temp[i] = word[i] | BITS; 
    }
    
    if (currentWordLength > 3)
    {
        //Set the first 5 values of the word to the first word array starting at the 5th indicy working down
        for (int i = 0; i < 3; i++)
        {
            currentWordFirst5[2 - i] = hash(&temp[i]);
        }
        traversalNode = tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]];
        while (traversalNode != NULL)
        {
            if (memcmp(temp, (traversalNode -> word), currentWordLength) == 0)
            {
                if (traversalNode -> word[currentWordLength + 1] != '\0')
                {
                    traversalNode = traversalNode -> next;
                }
                else
                {
                    //If we are not at the start of the list
                    if (traversalNode -> previous != NULL)
                    {
                        traversalNode -> previous -> next = traversalNode -> next;
                        if (traversalNode -> next != NULL)
                        {
                            traversalNode -> next -> previous = traversalNode -> previous;
                        }
                        //Set the current node's next to the start of the list
                        traversalNode -> next = tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]];
                        traversalNode -> previous = NULL;
                        //Set the start of the list to the current node
                        tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]] = traversalNode;
                    }
                    return true;   
                }

            }
            else
            {
                traversalNode = traversalNode -> next;
            }
        }
        return false;
    }
    else
    {
        //For the 5 dims of the array
        for (int i = 0; i < 3; i++)
        {
            //Hash the char at set to
            currentWordFirst5[2 - i] = (i < currentWordLength) ? hash(&temp[i]) + 1 : 0;
        }

        //Set the under five tables bool value to true
        return tableUnderFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]];
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    
    if (word[0] == '\'')
    {
        return 26;
    }
    else
    {
        //We can use bitwise here
        char temp = word[0] | BITS;
        return temp - 97;
        //Or the comparison - which is faster?
        //Case insensitive ASCII value for letter where A = 0
        // return ((word[0] - 97) < 0) ? word[0] - 65 : word[0] -  97;
    }

    //Used to check success
    return -50;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    struct stat inputFile_Stat;
    int index = 0;
    //Open the file
    FILE *dictPointer = fopen(dictionary, "r");
    if (dictPointer == NULL)
    {
        printf("Exiting, failed to read dictionary.\n");
        return false;
    }

    //Get files stats
    if (stat(dictionary, &inputFile_Stat) != 0)
    {
        printf("Exiting, failed to read dictionary file properties.\n");
        return false;
    }

    //Allocate space for dictionary in memory
    char *_dictionary;
    _dictionary = malloc((long long)inputFile_Stat.st_size + 1);
    if (_dictionary == NULL)
    {
        printf("Exiting, memory allocation fail on dictionary load");
        return false;
    }
    //Load dictionary to memory
    if (fread(_dictionary, inputFile_Stat.st_size, 1, dictPointer) != 1)
    {
        printf("Exiting, failed to load dictionary to memory");
        return false;
    }

    fclose(dictPointer);

    char *currentPos = _dictionary;
    char *wordStartPos = _dictionary;
    
    currentCollisionsList = CreateCollisionsList(currentWordFirst5);

    //While we're still in the dictionary
    while (currentPos < _dictionary + inputFile_Stat.st_size)
    {
        //Move currentPos to next backslash (end of word)
        currentPos = memchr(currentPos, '\n', inputFile_Stat.st_size);

        //wordStartPos to current pos is the word
        currentWordLength = currentPos - wordStartPos;

        if (currentWordLength > 3)
        {
            //Set the first 5 values of the word to the first word array starting at the 5th indicy working down
            for (int i = 0; i < 3; i++)
            {
                currentWordFirst5[2 - i] = hash(&wordStartPos[i]);
            }

            //Check the over five tables ptr for null
            if (tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]] == NULL)
            {
                tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]] = CreateNode(wordStartPos, currentWordLength);
                currentCollisionsList = AddCollisionsList(currentCollisionsList, currentWordFirst5);
            }
            else
            {
                tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]] = AddNode(
                            tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]], wordStartPos, currentWordLength);
            }
        }
        else
        {
            //For the 5 dims of the array
            for (int i = 0; i < 3; i++)
            {
                //Hash the char at set to
                currentWordFirst5[2 - i] = (i < currentWordLength) ? hash(&wordStartPos[i]) + 1 : 0;
            }

            //Set the under five tables bool value to true
            tableUnderFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]] = true;
        }

        //Increment pointers
        currentPos += 1;
        wordStartPos = currentPos;
        numberOfWords++;
    }
    free(_dictionary);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return numberOfWords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    while (currentCollisionsList -> next != NULL)
    {
  
        deleteHashtable(tableOverFive
                        [currentCollisionsList -> position[0]]
                        [currentCollisionsList -> position[1]]
                        [currentCollisionsList -> position[2]]
                       );
        
        previousCollisionsList = currentCollisionsList;
        currentCollisionsList = currentCollisionsList -> next;
        free(previousCollisionsList);
    }
    free(currentCollisionsList);
    return true;
}


//Creates a new node for a linked list that is null
node *CreateNode(char *wordToAdd, int wordLength)
{
    
    //Allocate memory for node
    node *temp = malloc(sizeof(node));
    if (temp == NULL)
    {
        printf("Unable to allocate space.\n");
        exit(1);
    }
    
    temp -> word[wordLength + 1] = '\0';
    //Changed strlen(wordToAdd) + 1 to word Lenght +1 watch for seg fault
    //Copy the word to the node
    memcpy(temp -> word, wordToAdd, wordLength + 1);
    temp -> next = NULL;
    temp -> previous = NULL;
    return temp;
}

// add a new node to an existing linked list, inlined for performance
node *AddNode(node *head, char *wordToAdd, int wordLength)
{
    //Allocate memory for node
    node *temp = malloc(sizeof(node));
    if (temp == NULL)
    {
        printf("Unable to allocated space.\n");
        exit(1);
    }
    
    temp-> word[wordLength + 1] = '\0';

    
    //Copy word to node
    memcpy(temp -> word, wordToAdd, wordLength + 1);
    temp -> next = head;
    temp -> previous = NULL;
    head -> previous = temp; 
    return temp;
}

//Creates a new node for a linked list that is null
collisionsList *CreateCollisionsList(int indicie[3])
{
    //Allocate memory for node
    collisionsList *temp = malloc(sizeof(collisionsList));
    if (temp == NULL)
    {
        printf("Unable to allocate space.\n");
        exit(1);
    }

    //Changed strlen(wordToAdd) + 1 to word Lenght +1 watch for seg fault
    //Copy the word to the node
    memcpy(temp -> position, indicie, sizeof(int[3]));
    temp -> next = NULL;
    return temp;
}

// add a new node to an existing linked list, inlined for performance
collisionsList *AddCollisionsList(collisionsList *head, int indicie[3])
{
    //Allocate memory for node
    collisionsList *temp = malloc(sizeof(collisionsList));
    if (temp == NULL)
    {
        printf("Unable to allocated space.\n");
        exit(1);
    }
    //Copy word to node
    memcpy(temp -> position, indicie, sizeof(int[3]));
    temp -> next = head;
    return temp;
}

void deleteHashtable(node *start)
{
    if (start == NULL)
    {
        return;
    }
    else
    {
        deleteHashtable(start -> next);
        start -> next = NULL;
        free(start);
    }
}


void deleteCollisionsList(collisionsList *start)
{
    if (start == NULL)
    {
        return;
    }
    else
    {
        deleteCollisionsList(start -> next);
        start -> next = NULL;
        free(start);
    }
}