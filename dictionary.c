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
    char word[LENGTH - 5];
    struct node *next;
}
node;

typedef struct list
{
    int position[5];
    struct list *next;
}
collisionsList;

node* CreateNode(char * wordToAdd, int wordLength);
collisionsList* CreateCollisionsList(int indicie[5]);

node* AddNode(node* head, char * wordToAdd, int wordLength);
collisionsList* AddCollisionsList(collisionsList* head, int indicie[5]);

void deleteHashtable(node* start);

// for bitwise OR to lowercase
#define BITS 32

//Hash table 0 - 25 = A - Z ' =  26
node *tableOverFive[27][27][27][27][27] = {{{{{NULL}}}}};

//Hash table 1 - 26 = A - Z 27 =  ' No char=  0
bool tableUnderFive[28][28][28][28][28];

node *traversalNode;

collisionsList* currentCollisionsList;
collisionsList* previousCollisionsList;

//Array to store current words first 5 ASCII values
int currentWordFirst5[5];
int currentWordLength;
int numberOfWords =0;
int collisionsNumber = 0;
int wordsLongerThan5 = 0;
float wordLengthTotal = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    
    currentWordLength = strlen(word);
    if (currentWordLength > 5)
    {
        //Set the first 5 values of the word to the first word array starting at the 5th indicy working down
        for (int i = 0; i < 5; i++)
        {
            currentWordFirst5[4 - i] = hash(&word[i]);
        }
        traversalNode = tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]];
        while (traversalNode != NULL)
        {
            
            if (memcmp(word, (traversalNode -> word), currentWordLength) == 0 && traversalNode -> word[currentWordLength + 1] == '\0')
            {
                
                    return true;
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
        for (int i = 0; i < 5; i++)
        {
            //Hash the char at set to
            currentWordFirst5[4 - i] = (i < currentWordLength) ? hash(&word[i]) + 1 : 0;
        }

        //Set the under five tables bool value to true
        return tableUnderFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]];
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
    FILE* dictPointer = fopen(dictionary, "r");
    if (dictPointer == NULL)
    {
        printf("Exiting, failed to read dictionary.\n");
        return false;
    }

    //Get files stats
    if (stat(dictionary, &inputFile_Stat)!= 0)
    {
        printf("Exiting, failed to read dictionary file properties.\n");
        return false;
    }

    //Allocate space for dictionary in memory
    char *_dictionary;
    _dictionary = malloc((long long)inputFile_Stat.st_size + 1);
    if(_dictionary == NULL)
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
        wordLengthTotal += (float)currentWordLength;

        if (currentWordLength > 5)
        {
            wordsLongerThan5++;
            //Set the first 5 values of the word to the first word array starting at the 5th indicy working down
            for (int i = 0; i < 5; i++)
            {
                currentWordFirst5[4 - i] = hash(&wordStartPos[i]);
            }

            //Check the over five tables ptr for null
            if(tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]]== NULL)
            {
                tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]] = CreateNode(wordStartPos, currentWordLength);
                collisionsNumber++;
                currentCollisionsList = AddCollisionsList(currentCollisionsList, currentWordFirst5);
            }
            else
            {
                tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]] = AddNode(tableOverFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]], wordStartPos, currentWordLength);
                collisionsNumber++;
            }
        }
        else
        {
            //For the 5 dims of the array
            for (int i = 0; i < 5; i++)
            {
                //Hash the char at set to
                currentWordFirst5[4 - i] = (i < currentWordLength) ? hash(&wordStartPos[i]) + 1 : 0;
            }

            //Set the under five tables bool value to true
            tableUnderFive[currentWordFirst5[0]][currentWordFirst5[1]][currentWordFirst5[2]][currentWordFirst5[3]][currentWordFirst5[4]] = true;
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
        [currentCollisionsList -> position[3]]
        [currentCollisionsList -> position[4]]
        );
        
        previousCollisionsList = currentCollisionsList;
        currentCollisionsList = currentCollisionsList -> next;
        free(previousCollisionsList);
    }
    free(currentCollisionsList);
    /*
    float averageWordLength = wordLengthTotal / numberOfWords;
    printf("Collisisons: %i", collisionsNumber);
    printf("Words Longer Than 5: %i", wordsLongerThan5);
    printf("Average word length: %f", averageWordLength);
    */
    return true;
}


//Creates a new node for a linked list that is null
node* CreateNode(char * wordToAdd, int wordLength)
{
    //Allocate memory for node
    node* temp = malloc(sizeof(node));
    if (temp == NULL)
    {
        printf("Unable to allocate space.\n");
        exit(1);
    }

    //Changed strlen(wordToAdd) + 1 to word Lenght +1 watch for seg fault
    //Copy the word to the node
    memcpy(temp -> word, wordToAdd, wordLength + 1);
    temp -> next = NULL;
    return temp;
}

// add a new node to an existing linked list, inlined for performance
node* AddNode(node* head, char * wordToAdd, int wordLength)
{
    //Allocate memory for node
    node* temp = malloc(sizeof(node));
    if (temp == NULL)
    {
        printf("Unable to allocated space.\n");
        exit(1);
    }
    //Copy word to node
    memcpy(temp -> word, wordToAdd, wordLength + 1);
    temp -> next = head;
    return temp;
}

//Creates a new node for a linked list that is null
collisionsList* CreateCollisionsList(int indicie[5])
{
    //Allocate memory for node
    collisionsList* temp = malloc(sizeof(collisionsList));
    if (temp == NULL)
    {
        printf("Unable to allocate space.\n");
        exit(1);
    }

    //Changed strlen(wordToAdd) + 1 to word Lenght +1 watch for seg fault
    //Copy the word to the node
    memcpy(temp -> position, indicie, 40);
    temp -> next = NULL;
    return temp;
}

// add a new node to an existing linked list, inlined for performance
collisionsList* AddCollisionsList(collisionsList* head, int indicie[5])
{
    //Allocate memory for node
    collisionsList* temp = malloc(sizeof(collisionsList));
    if (temp == NULL)
    {
        printf("Unable to allocated space.\n");
        exit(1);
    }
    //Copy word to node
    memcpy(temp -> position, indicie, 40);
    temp -> next = head;
    return temp;
}

void deleteHashtable(node* start)
{
    if(start == NULL)
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


void deleteCollisionsList(collisionsList* start)
{
    if(start == NULL)
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