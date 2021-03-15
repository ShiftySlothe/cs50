#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "dictionary.h"

const int dictionaryLength = 466550;
const int longestWordLgth = 45;

int main(int argc, char *argv[])
{
    
    char* dictionary  = argv[1];
    char words[dictionaryLength][longestWordLgth];
    char wordsCondensed[dictionaryLength][longestWordLgth];
    for (int x = 0; x <dictionaryLength; x++)
    {
        for (int y = 0; y < dictionaryLength; y++)
        {
            words[x][y] = 0;
            wordsCondensed[x][y] = 0;
        }
    }
    FILE* dictPointer = fopen(dictionary, "r");
    if (dictPointer == NULL)
    {
        printf("Exiting, failed to read dictionary.\n");
        return false;
    }
    
    struct stat inputFile_Stat;
    
    //Get files stats
    if (stat(dictionary, &inputFile_Stat)!= 0)
    {
        printf("Exiting, failed to read dictionary file properties.\n");
        return false;
    }
    
    //Allocate space for dictionary in memory
    char *pt_dictionary;
    pt_dictionary = malloc((long long)inputFile_Stat.st_size + 1);
    if(pt_dictionary == NULL)
    {
        printf("Exiting, memory allocation fail on dictionary load");
        return false;
    }
    
    //Load dictionary to memory
    if (fread(pt_dictionary, inputFile_Stat.st_size, 1, dictPointer) != 1)
    {
        printf("Exiting, failed to load dictionary to memory");
        return false;
    }
    
    fclose(dictPointer);
    
    char *where_at = pt_dictionary;
    char *word_start = NULL; 
    char slash = 10;
    int numberOfWords = 0; 
    int wordLengthCounter = 0;
    int condensedWordCounter = -1; 
    int longestWordLength = 0;
    int previousWordLength = 0;
  
    while (where_at < pt_dictionary + inputFile_Stat.st_size)
    {
        bool lastWordSame = true;
        word_start = memchr(where_at, slash, 1);
        if (word_start == NULL)
        {
            words[numberOfWords][wordLengthCounter] = *where_at;
            wordLengthCounter++;
            where_at++;
        }
        else
        {
            where_at++; 
            longestWordLength = (wordLengthCounter > longestWordLength) ? wordLengthCounter : longestWordLength;
            //If the previous word length is > the current word
            if (previousWordLength > wordLengthCounter || numberOfWords == 0)
            {
                //Increment counter
                condensedWordCounter++;
                //Add the current word to the array as a new word
                for (int i = 0; i < wordLengthCounter; i++)
                {
                    wordsCondensed[condensedWordCounter][i] = words[numberOfWords][i];
                }
            }
            else
            {
                //If the previous word matches the current word for the length of the previous word
                for (int i = 0; i < previousWordLength; i++)
                {
                    lastWordSame = (words[numberOfWords - 1][i] != words[numberOfWords][i]) ? false : true;
                }
                if (lastWordSame)
                {
                    //Replace the last word with the current word
                    for (int i = 0; i < wordLengthCounter; i++)
                    {
                        wordsCondensed[condensedWordCounter][i] = words[numberOfWords][i];
                    } 
                }
                else
                {
                    //Increment counter
                    condensedWordCounter++;
                    //Add the current word to the array as a new word
                    for (int i = 0; i < wordLengthCounter; i++)
                    {
                        wordsCondensed[condensedWordCounter][i] = words[numberOfWords][i];
                    }
                }
            }
            previousWordLength = wordLengthCounter;
            wordLengthCounter = 0;
            numberOfWords++;
        }
    }
    
    printf("Words Found\n");
    for (int k = 0; k < dictionaryLength; k++)
    {
        for (int l = 0; l < longestWordLgth; l++)
        {
            printf("%c", words[k][l]);
        }
        printf("\n");

    }
    printf("---------------------------------\n");
    printf("Words condensed\n");
    

    for (int k = 0; k < dictionaryLength; k++)
    {
        for (int l = 0; l < longestWordLgth; l++)
        {
            if( wordsCondensed[k][l] != 0)
            {
                printf("%c", wordsCondensed[k][l]);
            }
        }

        printf("\n");
        

    }
    printf("\n");
    printf("Number of words: %i\n", numberOfWords);
    printf("Longest word: %i\n", longestWordLength);
    
    free(pt_dictionary);
}