#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int sizeOfTower;
    
    //Requests input until corect 
    do
    {
        sizeOfTower = get_int("How tall do you want your tower? (1 - 8)");
    }
    while (sizeOfTower > 8 || sizeOfTower < 1);

    //For each row
    for (int i = 1; i <= sizeOfTower; i++)
    {
        //Draw spaces
        for (int j = sizeOfTower - i - 1; j >= 0; j--)
        {
            printf(" ");
        }
        //Draw blocks
        for (int k = i; k > 0; k--)
        {
            printf("#");
        }
        //Print 2 spaces 
        printf("  ");
        //Draw same number of blocks
        for (int l = i; l > 0; l--)
        {
            printf("#");
        }
        //Go to the next line
        printf("\n");
    }
}