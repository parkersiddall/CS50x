#include<cs50.h>
#include<stdio.h>
#include<ctype.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, string argv[]) //command line argument
{
    if (argc == 2) //when it is two words, first word being the file name
    {
        for (int i = 0; argv[1][i]; i++) //loop to go through each letter in the second string of the argv array
        {
            if isdigit (argv[1][i]) //is it a digit?
            {
                // do nothing and go on to the next char in the string
            }
            else
            {
                printf("Usage: ./caesar key\n"); //prompt user for only digits
                return 1; //return error message
            }
        }

        int x = atoi(argv[1]); //convert string to int, declare its value as "x"
        printf("%i", x);

        string pt = get_string("plaintext: "); //prompt user for input
        printf("ciphertext: "); //print "cyphertext", no new line

        for (int j = 0; pt[j]; j++)
        {
            if isalpha(pt[j]) //check to make sure it is an alphabetical character. I can probably remove this...
            {
                if isupper(pt[j])
                {
                    printf("%c", (((char) pt[j] + (int) x - 65) % 26) + 65);
                }
                else if islower(pt[j])
                {
                    printf("%c", (((char) pt[j] + (int) x - 97) % 26) + 97);
                }
            }
            else
            {
                printf ("%c", (char) pt[j]);
            }
        }
        printf("\n"); //print new line at end to make it look nice
    }

    else //if more than 2 words are inserted in the command line
    {
        printf("Incorrect input, try again.\n");
        return 1;
    }
}