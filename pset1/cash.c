#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    //declare variables
    int quarter;
    int dime;
    int nickel;
    int penny;
    int changeoutput;

    float getchange;

    //prompt for change float
    do
    {
        float getchange = get_float("Amount of change: ");
    }
    
    //create loop until positive number is created
    while (getchange > 0);

    //convert to cents
    int cents = round(getchange * 100)

    //loop for quarters
    for (quarter = 1; cents >= 25; quarter++)
    {
        cents = cents - 25;
        changeoutput = change output + 1;
    }

    //loop for dimes
    for (dime = 1; cents >= 10; dime++)
    {
        cents = cents - 10;
        changeoutput = changeoutput + 1;
    }

    //loop for nickels
    for (nickel = 1; cents >= 5; nickel++)
    {
        cents = cents - 5;
        changeoutput = changeoutput + 1;
    }

    //loop for pennies
    for (penny = 1; cents >= 1; penny++)
    {
        cents = cents - 1;
        changeoutput = changeoutput + 1;
    }

    //print output
    printf("%i\n", changeoutput);
    
}
