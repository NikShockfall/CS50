#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

//string alphabet = "abcdefghijklmnopqrstuvwxyz";

bool badkey(string s);
string cipher(string key, string s);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    if (badkey(key))
    {
        return 1;
    }


    string text = get_string("plaintext: ");
    printf("ciphertext: %s\n", cipher(key, text));

    return 0;
}

bool badkey(string s)
{
    if (strlen(s) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return true;
    }

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if(isalpha(s[i]) == 0)
        {
            printf("Key must only contain alphabetic characters.\n");
            return true;
        }

        if(strrchr(s,s[i]) != &s[i])
        {
            printf("Key must not contain repeated characters.\n");
            return true;
        }
    }

    return false;
}

string cipher(string key, string s)
{
    int dex;

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if(isalpha(s[i]))
        {
            if (isupper(s[i]))
            {
                dex = s[i] - 65;
                s[i] = toupper(key[dex]);
            }

            else
            {
                dex = s[i] - 97;
                s[i] = tolower(key[dex]);
            }
        }
    }

    return s;
}
