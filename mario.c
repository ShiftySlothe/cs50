#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int sizeOfTower;
    
    //This solution appears to work 
    do{
      sizeOfTower = get_int("How big do you want your tower? ");
    }
    while(sizeOfTower > 9 || sizeOfTower < 0);

    for (int i = 1; i <= sizeOfTower; i++)
    {
        for (int j = sizeOfTower - i - 1; j >= 0; j--)
        {
            printf(" ");
        }

        for(int k = i; k > 0; k--)
        {
            printf("#");
        }
        printf("  ");
        for(int l = i; l > 0; l--)
        {
            printf("#");
        }
        printf("\n");
    }
}