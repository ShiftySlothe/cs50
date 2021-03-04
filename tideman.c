#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
int GetWinningMargin(pair _pair);
void lock_pairs(void);
bool propose_link(int linkee, int originalCandidate);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //For each candidate(i) in ranks (bar the last)
    for (int i = 0; i < candidate_count - 1; i++)
    {
        //For every candidate(j) found after that candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            //Add 1 to the prefences array at (i,j)
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //For each candidate(i) in preferences
    for (int i = 0; i < candidate_count; i++)
    {
        //For each opposing candidate
        for (int j = 0; j < candidate_count; j++)
        {
            //If voters prefered i to j
            if (preferences[i][j] > preferences[j][i])
            {
                pair tempPair;
                //Set the winner and looser
                tempPair.winner = i;
                tempPair.loser = j;
                //Add the pair to the pairs array
                pairs[pair_count] = tempPair;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swapCounter = -1;
    while(swapCounter != 0)
    {
        swapCounter = 0;
        //For each pair in pairs (bar the last)
        for (int i = 0; i < pair_count - 1; i++)
        {
            int pair1 = GetWinningMargin(pairs[i]);
            int pair2 = GetWinningMargin(pairs[i + 1]);

            //If the margin in smaller than the next pair
            if (GetWinningMargin(pairs[i]) < GetWinningMargin(pairs[i + 1]))
            {
                //Swap the pairs
                swapCounter++;
                pair temp = pairs[i + 1];
                pairs[i + 1] =  pairs[i];
                pairs[i] = temp;
            }
        }
    }

    return;
}

//Given a pair, calculate the winning margin.
int GetWinningMargin(pair _pair)
{
    int winner = _pair.winner;
    int looser = _pair.loser;

    //Take the loosers number of individual victories off the winners
    return preferences[winner][looser] - preferences[looser][winner];

}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //For each pair
    for (int i = 0; i < pair_count; i++)
    {
        //Set the link to true if propose link returns true
        locked[pairs[i].winner][pairs[i].loser] = propose_link(pairs[i].loser, pairs[i].winner);
    }
    return;
}

bool propose_link(int linkee, int originalCandidate)
{
    bool nonCircular = true;
    //For each potential link from the pairs looser
    for (int i = 0; i < MAX; i++)
    {
        //If there is an onward link
        if (locked[linkee][i] == true)
        {
            //If the link links back to origianl
            if (i == originalCandidate)
                return false;
            //Check any onward links until there are none
            nonCircular = propose_link(i, originalCandidate);
        }
    }
    //If there are no onward links, return true
    return nonCircular;
}

// Print the winner of the election
void print_winner(void)
{
    int winner[MAX];
    int k = 0;
    //For each potential candidate
    for (int i = 0; i < candidate_count; i++)
    {
        bool linksPresent = false;
        //For each potential link to them
        for (int j = 0; j < candidate_count; j++)
        {
            linksPresent = locked[j][i];
        }
        if(linksPresent)
        {
            winner[k] = i;
            k++;
        }

    }
    
    for (int l = k; l > -1; l--)
    {
         printf("%s\n", candidates[winner[l]]);   
    }
}

