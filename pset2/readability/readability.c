#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int main(void)
{
    //get string
    string text = get_string("text: ");

    //declare counters
    int chars = 0;
    int words = 1; //plus 1 because at the end of the final sentence there is no space
    int sents = 0;

    //count characters
    for (int i = 0; i <= strlen (text); i++)
    {
        if (text[i] >= 65 && text[i] <= 122) //instert number range that includes caps and lowercase letters
        {
            chars++;
        }
    }
   
    //count words
    for (int j = 0; j <= strlen (text); j++)
    {
        if (text[j] == 32)
        {
            words++;
        }
    }
   
    //count sentences
    for (int k = 0; k <= strlen (text); k++)
    {
        if (text[k] == 46 || text[k] == 63 || text[k] == 33)
        {
            sents++;
        }
    }
    
    //putting it all together
    //decare L and S values
    float l = ((float) chars / (float) words) * 100;
    float s = ((float) sents / (float) words) * 100;
    
    //Scholastic readability equation
    float index = 0.0588*l - 0.296*s - 15.8;
    
    index = round(index); //round to nearest whole number
    
    if (index < 1) //for before grade 1
    {
        printf("Before Grade 1\n");
    }
    
    else if (index > 16) // above grade 16
    {
        printf("Grade 16+\n");
    }
    else //print grade index
    {
        printf("Grade %i\n", (int) index);
    }
}