#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
void sortWinners();

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }


    sortWinners();

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }

    return false;
}

void sortWinners()
{
    int swapCounter = -1;
    int j = 0;
    while(swapCounter != 0)
    {
        swapCounter = 0;
        for(int i = 1; i < candidate_count - j; i++)
        {
            if (candidates[i - 1].votes > candidates[i].votes)
            {
                swapCounter++;
                candidate temp =  candidates[i];
                candidates[i] = candidates[i - 1];
                candidates[i - 1] = temp;
            }
            j++;
        }
    }

}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int i = 1;
    int highscore = candidates[candidate_count - i].votes;
    printf("%s\n", candidates[candidate_count - i].name);
    while (candidates[candidate_count - (i + 1)].votes >= highscore)
    {
       i++;
       printf("%s\n", candidates[candidate_count - i].name);
    }

}
