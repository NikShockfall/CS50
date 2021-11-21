#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Ask user's name as input
    string answer = get_string("What's your name? ");

    //Greet user
    printf("Hello, %s\n", answer);
}