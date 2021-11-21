#include <cs50.h>
#include <stdio.h>

void blocks(int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    if (n == 1)
    {
       printf("#  #\n");
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                printf(" ");
            }

            blocks(i + 1);
            printf("  ");
            blocks(i + 1);
            printf("\n");
        }
    }
}

void blocks(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}