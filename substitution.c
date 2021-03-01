#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int ofset[26];
int cypherLength;
string userInputCypher;
int stringLength;
string userInputPlainText;
char userOutputCypher[26];

bool CheckUserInput();
void ToUpperCase();
void MoveToArray();
void CalculateASCIIOfset();
void EncodePlainText();

int main(int args, char *cypherInput[])
{
    if (args == 1)
    {
        printf("Invalid or empty argument provided \n");
        return 1;
    }

    userInputCypher = cypherInput[0];
    stringLength = strlen(userInputCypher);
    ToUpperCase();

    // MoveToArray
    if (!CheckUserInput())
    {
        printf("Error, incorrect format.");
        return 1;
    }

    CalculateASCIIOfset();

    userInputPlainText = get_string("plaintext:");

    EncodePlainText();

    printf("%s", userOutputCypher);
}

void ToUpperCase()
{
    for(int i = 0; i < stringLength; i++)
    {
        userInputCypher[i] = toupper(userInputCypher[i]);
    }
}
bool CheckUserInput()
{
    cypherLength = strlen(userInputCypher);

    //Check length
    if (stringLength != 26)
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

void EncodePlainText()
{
    for (int i = 0; i < 26; i++)
    {
        if (userInputCypher[i] >= 65 && userInputCypher[i] <= 90)
        {
          int cypherASCIIValue = userInputCypher[i] - 0;
          userOutputCypher[i] = cypherASCIIValue - ofset[i];
        }
          if (userInputCypher[i] >= 97 && userInputCypher[i] <= 122)
        {
          int cypherASCIIValue = userInputCypher[i] - 0;
          userOutputCypher[i] = cypherASCIIValue - ofset[i];
        }
    }
}
