#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define WORDS 100.0

int get_words(string s);
int get_alpha(string s);
int get_sentence(string s);
int get_index(int w, int l, int s);

int main(void)
{
    string text = get_string("Text: ");

    int words = get_words(text);
    int letters = get_alpha(text);
    int sentences = get_sentence(text);
    printf("===%i letters, %i words, %i sentences===\n", letters, words, sentences);

    // index = 0.0588 * L - 0.296 * S - 15.8
    int index = get_index(words, letters, sentences);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index <= 16)
    {
        printf("Grade %i\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }

}

int get_words(string s)
{
    int num = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == ' ')
        {
            num++;
        }
    }

    return ++num;
}

int get_alpha(string s)
{
    int num = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isalpha(s[i]))
        {
            num++;
        }
    }

    return num;
}

int get_sentence(string s)
{
    int num = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == '?' || s[i] == '.' || s[i] == '!')
        {
            num++;
        }
    }

    return num;
}

int get_index(int w, int l, int s)
{
    float w1 = WORDS/w;
    float index = 0.0588 * l * w1 - 0.296 * s * w1 - 15.8;

    return round(index);
}