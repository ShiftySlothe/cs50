#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startSize = 0;
    int endSize = 0;
    int numberOfYears = 0;
    //Prompt for start size
    do
    {
        startSize = get_int("Population start size: ");
    }
    while (startSize < 1 || startSize >= 10);
    // Prompt for end size
    do
    {
        endSize = get_int("Population end size: ");
    }
    while (endSize < startSize);
    
    // Calculate number of years until we reach threshold
    while (startSize < endSize)
    {
        numberOfYears++;
        startSize = startSize + (startSize / 3) - (startSize / 4);
    }

    // Print number of years
    printf("Years : %i\n", numberOfYears);
}