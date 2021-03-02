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
void mergeSort(candidate startingArray[], int left, int right);
void merge(candidate startingArray[], int left, int middle, int right);

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

    //Merge sort array
    mergeSort(candidates, 0, candidate_count -1);

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
//Left is for left index, right for right of subarray to be sorted
void mergeSort(candidate startingArray[], int left, int right)
{
    if(left < right)
    {
        int middle = left + (right - 1) / 2;

        mergeSort(startingArray, left, middle);
        mergeSort(startingArray, middle + 1, right);

        merge(startingArray, left, middle, right);
    }
}

void merge(candidate startingArray[], int left, int middle, int right)
{
    //Temp variables for loops
    int i, j, k;

    //Get length of each sub array
    int leftLength = middle - left + 1;
    int rightLength = right - middle;

    //Create temporarry arrays
    candidate leftHalf[leftLength], rightHalf[rightLength];

    //Copy data from arrays to temp
    for (i = 0; i < leftLength; i++)
    {
        leftHalf[i] = startingArray[left + i];
    }

    for (j = 0; j < rightLength; j++)
    {
        rightHalf[j] = startingArray[middle + 1 + j];
    }

    //Merge the temporary arrays back into the main array
    i = 0; //Initial index of leftHalf
    j = 0; //Initial index of rightHalf
    k = left; //Initial index of merged subarray

    while (i < leftLength && j < rightLength)
    {
        if (leftHalf[i].votes <= rightHalf[j].votes)
        {
            startingArray[k] = leftHalf[i];
            i++;
        }
        else
        {
            startingArray[k] = rightHalf[j];
            j++;
        }
        k++;
    }

    //Copy any remaining elements of sub array if present
    while (i < leftLength)
    {
        startingArray[k] = leftHalf[i];
        i++;
        k++;
    }

    while (j < rightLength)
    {
        startingArray[k] = rightHalf[j];
        j++;
        k++;
    }
}