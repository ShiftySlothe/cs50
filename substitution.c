#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Empty array, 1's are added once a character is included in Cypher
int charCheck[26];
//ASCII ofset of cypher to correct character in alphabet
int ofset[26];

string userInputCypher;
int cypherLength;

string userInputPlainText;
int stringLength;

//Ensures length, letters and lack of repitition in input.
bool CheckUserInput();
void ToUpperCase();
void CalculateASCIIOfset();
string EncodePlainText();

int main(int argc, string argv[])
{
    //Checks command line arguments
    if (argc == 1)
    {
        printf("No argument provided \n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("Too many arguments provided. \n");
        return 1;
    }
    
    userInputCypher = argv[1];
    cypherLength = strlen(userInputCypher);
    ToUpperCase();

    //Checks user input for errors
    if (!CheckUserInput())
    {
        printf("Error, incorrect format.\n");
        return 1;
    }

    CalculateASCIIOfset();
    
    userInputPlainText = get_string("plaintext:");
    stringLength = strlen(userInputPlainText);
    
    //Assigns a string based on user input length
    string userOutputCypher = malloc(stringLength);
    userOutputCypher = EncodePlainText();

    printf("ciphertext: %s", userOutputCypher);
    printf("\n");
    return 0;
}

void ToUpperCase()
{
    for (int i = 0; i < cypherLength; i++)
    {
        userInputCypher[i] = toupper(userInputCypher[i]);
    }
}

bool CheckUserInput()
{
    cypherLength = strlen(userInputCypher);

    //Check length
    if (cypherLength != 26)
    {
        return false; 
    }

    //Check each character
    for (int i = 0; i < 26; i++)
    {
        if (userInputCypher[i] < 65 || userInputCypher[i] > 90)
        {
            return false;
        }
        //Check for repeated characters
        else
        {
            if (charCheck[userInputCypher[i] - 0] == 1)
            {
                return false;
            }
            charCheck[userInputCypher[i] - 0] = 1;
        }
    }

    return true;
}

void CalculateASCIIOfset()
{
    for (int i = 0; i < 26; i++)
    {
        //Get ASCII value
        int cypherASCIIValue = userInputCypher[i] - 0;
        //Iterate through alphabet
        int alphabeticalASCIIValue = i + 65;
        //Caculate ofset
        ofset[i] = cypherASCIIValue - alphabeticalASCIIValue;
    }
}

string EncodePlainText()
{
    string output = malloc(stringLength);
    
    //Iterate through each letter in input
    for (int i = 0; i < stringLength; i++)
    {
        //Upper case
        if (userInputPlainText[i] >= 65 && userInputPlainText[i] <= 90)
        {
            int cypherASCIIValue = userInputPlainText[i] - 0;
            output[i] = cypherASCIIValue + ofset[cypherASCIIValue - 65];
        }
        
        //Lower case
        else if (userInputPlainText[i] >= 97 && userInputPlainText[i] <= 122)
        {
            int cypherASCIIValue = userInputPlainText[i] - 0;
            output[i] = cypherASCIIValue + ofset[cypherASCIIValue - 97];
        }
        //Punctuation
        else
        {
            output[i] = userInputPlainText[i];
        }
    }
    
    return output;
}
