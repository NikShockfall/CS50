#include <stdio.h>
#include <cs50.h>

void printchar(char c, int num);

int main(void)
{
    int n;
    //Enter a positive integer
    do
    {
        n = get_int("Height: \n");
    }
    while (n < 1 || n > 8);

    //Processing
    int x = 1;
    while (x <= n)
        //Print single row of " "
    {
        printchar(' ', n - x);
        printchar('#', x);
        printf("\n");
        x++;
    }
}

void printchar(char c, int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("%c", c);
    }
}