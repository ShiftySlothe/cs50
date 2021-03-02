#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int charCheck[26];
int ofset[26];
int cypherLength;
string userInputCypher;
int stringLength;
string userInputPlainText;


bool CheckUserInput();
void ToUpperCase();
void MoveToArray();
void CalculateASCIIOfset();
string EncodePlainText();

int main(int argc, string argv[])
{
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

    // MoveToArray
    if (!CheckUserInput())
    {
        printf("Error, incorrect format.\n");
        return 1;
    }

    CalculateASCIIOfset();
    
    userInputPlainText = get_string("plaintext:");
    stringLength = strlen(userInputPlainText);
    
    string userOutputCypher = malloc(stringLength);
    userOutputCypher = EncodePlainText();

    printf("ciphertext: %s", userOutputCypher);
    printf("\n");
    return 0;
}

void ToUpperCase()
{
    for(int i = 0; i < cypherLength; i++)
    {
        userInputCypher[i] = toupper(userInputCypher[i]);
    }
}
bool CheckUserInput()
{
    cypherLength = strlen(userInputCypher);

    //Check length
    if (cypherLength != 26)
        return false;

    //Check each character
    for (int i = 0; i < 26; i++)
    {
        if (userInputCypher[i] < 65 || userInputCypher[i] > 90)
            return false;
    }

    return true;
}

void CalculateASCIIOfset()
{
    for (int i = 0; i < 26; i++)
    {
        int cypherASCIIValue = userInputCypher[i] - 0;
        int alphabeticalASCIIValue = i + 65;
        ofset[i] = cypherASCIIValue - alphabeticalASCIIValue;
    }
}

string EncodePlainText()
{
    string output = malloc(stringLength);
    
    for (int i = 0; i < stringLength; i++)
    {
        if (userInputPlainText[i] >= 65 && userInputPlainText[i] <= 90)
        {
          int cypherASCIIValue = userInputPlainText[i] - 0;
          output[i] = cypherASCIIValue + ofset[cypherASCIIValue - 65];
        }
        else if (userInputPlainText[i] >= 97 && userInputPlainText[i] <= 122)
        {
          int cypherASCIIValue = userInputPlainText[i] - 0;
          output[i] = cypherASCIIValue + ofset[cypherASCIIValue - 97];
        }
        else
        {
            output[i] = userInputPlainText[i];
        }
    }
    
    return output;
}
