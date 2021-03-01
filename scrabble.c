#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
string LETTERS[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    (score1 == score2) ? printf("Tie!\n") : (score1 > score2) ? printf("Player 1 Wins!\n") : printf("Player 2 Wins!\n");
}

int compute_score(string word)
{
    int score = 0;
    
    for(int i = 0, length = strlen(word); i < length; i++)
    {
        //Get each character of the word
        char currentLetter = word[i];
        //Force upper case
        currentLetter = toupper(currentLetter);
        //Using ASCII value, make A = 0 and Z = 25 
        int currentLetterValue = currentLetter - 65;
        //If the Char is between those two numbers, add to score
        if(currentLetterValue >= 0 && currentLetterValue < 26)
        {
            score += POINTS[currentLetterValue];
        }
    }
    return score;
}