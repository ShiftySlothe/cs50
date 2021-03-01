#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

string userInput;

float CalculateReadability();

int main(void)
{
    userInput = get_string("Text: ");
    
    float unroundedScore = CalculateReadability();
    int roundedScore = roundf(unroundedScore);

    if (roundedScore < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (roundedScore >= 1 && roundedScore < 16)
    {
        printf("Grade %i\n", roundedScore);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

float CalculateReadability()
{
    float letterCount = 0;
    //Initialised at 1 to account for the final word
    float wordCount = 1;
    float sentenceCount = 0;

    for (int i = 0, length = strlen(userInput); i < length; i++)
    {
        char currentChar = userInput[i];
        currentChar = toupper(currentChar);
        int currentCharIndex = currentChar - 0;
        
        //Checks to see if the char is within the ASCII bounds of A - Z
        if (currentCharIndex >= 65 && currentCharIndex <= 90)
        {
            letterCount++;
        }
        //Checks for a space
        if (currentCharIndex == 32)
        {
            wordCount++;
        }
        //Checks for ! . ?
        if (currentCharIndex == 33 || currentCharIndex == 46 || currentCharIndex == 63)
        {
            sentenceCount++;
        }
    }
    
    float wordsPer100 = wordCount / 100;
    //Number of letters per 100 words
    float lettersPer100 = letterCount / wordsPer100;
    //Number of sentences per 100 words
    float sentencesPer100 = sentenceCount / wordsPer100;
    
    //Coleman-Liau index
    return (0.0588 * lettersPer100) - (0.296 * sentencesPer100) - 15.8;
}
