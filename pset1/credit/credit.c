#include <cs50.h>
#include <stdio.h>

#define MIN 4000000000000
#define MAX 5600000000000000

int validate(long card);

int main(void)
{
    // Prompts user for a card number
    long cardNum = get_long("Number: ");

    int value = validate(cardNum);

    // Prints depending on the returned value.
    if (value / 10 == 4)
    {
        printf("VISA\n");
    }
    else if (value > 50 && value < 56)
    {
        printf("MASTERCARD\n");
    }
    else if (value == 34 || value == 37)
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
    }

}

// Process to validate cards, returns the first two card digits if valid
int validate(long card)
{
    // Detects if the number is outside the acceptable card range
    if (card < MIN || card > MAX)
    {
        return 1;
    }

    int len = 16, num = 0;

    // Converts card number to an array to access each digit
    int cards[len];
    for (int i = len - 1; i >= 0; i--)
    {
        cards[i] = card % 10;
        card /= 10;
    }

    //
    for (int i = 0; i < len; i++)
    {
        if (i % 2 == 1)
        {
            num += cards[i];
        }

        else
        {
            num += cards[i] * 2;

            if (cards[i] > 4)
            {
                num -= 9;
            }
        }

    }

    // Decides to return the first two card digits if the card is valid
    if (num % 10 == 0)
    {
        for (int i = 0; i < len; i++)
        {
            if (cards[i] != 0)
            {
                num = cards[i]*10 + cards[i+1];
                i = len;
            }
        }
    }
    else
    {
        num = 1;
    }

    return num;
}