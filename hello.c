#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get user input
    string name = get_string("What is your name human?\n");
    //Print output
    printf("Hello, %s \n", name);
}
