#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //create loop to go through image height
    for (int i = 0; i < height; i++)
    {
        //create loop to go through image width
        for (int j = 0; j < width; j++)
        {
            //create a variable which will save the average of the RGB values
            double average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtRed = round(average);

        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //create loop to go through image height
    for (int i = 0; i < height; i++)
    {
        //create loop to go through image width
        for (int j = 0; j < width; j++)
        {
            //create variables that determine the new sepia tones for RGB and cap them at 255
            double sepiaRed = (.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            double sepiaGreen = (.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            double sepiaBlue = (.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            //set original values to sepia tones
            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
     //create loop to go through image height
    for (int i = 0; i < height; i++)
    {
        //create loop to go through image width
        for (int j = 0; j < width; j++)
        {
        
        //declare a point to stop halfway
        int halfofwidth = round(width / 2);
        
        //start switching until we arrive at the halfway image
        while (j < halfofwidth)
        {
            RGBTRIPLE placeholder = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = placeholder;
            break;
        }
            
        }
    }
 
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
//Store copy of original
    RGBTRIPLE Original[height][width];
    
    //loop through rows
    for (int i = 0; i < height; i++)
    {
        //loop through columns
        for (int n = 0; n < width; n++) 
        {
            Original[i][n] = image[i][n];
        }
    }

    //loop through rows
    for (int i = 0; i < height; i++)
    {
        //loop through columns
        for (int n = 0; n < width; n++)
        {
            int counter = 0;
            float newred = 0;
            float newgreen = 0;
            float newblue = 0;
            
            if (i >= 0 && n >= 0)
            {
                newred = newred + Original[i][n].rgbtRed;
                newgreen = newgreen + Original[i][n].rgbtGreen;
                newblue = newblue + Original[i][n].rgbtBlue;
                counter++;
            }
            if (i >= 0 && n - 1 >= 0)
            {
                newred = newred + Original[i][n-1].rgbtRed;
                newgreen = newgreen + Original[i][n-1].rgbtGreen;
                newblue = newblue + Original[i][n-1].rgbtBlue;
                counter++;
            }
            if (i - 1 >= 0 && n >= 0)
            {
                newred = newred + Original[i-1][n].rgbtRed;
                newgreen = newgreen + Original[i-1][n].rgbtGreen;
                newblue = newblue + Original[i-1][n].rgbtBlue;
                counter++;
            }
            if (i >= 0  && n + 1 < width)
            {
                newred = newred + Original[i][n+1].rgbtRed;
                newgreen = newgreen + Original[i][n+1].rgbtGreen;
                newblue = newblue + Original[i][n+1].rgbtBlue;
                counter++;
            }
            if (i + 1 < height && n >= 0)
            {
                newred = newred + Original[i+1][n].rgbtRed;
                newgreen = newgreen + Original[i+1][n].rgbtGreen;
                newblue = newblue + Original[i+1][n].rgbtBlue;
                counter++;
            }
            if (i + 1 < height && n - 1 >= 0)
            {
                newred = newred + Original[i+1][n-1].rgbtRed;
                newgreen = newgreen + Original[i+1][n-1].rgbtGreen;
                newblue = newblue + Original[i+1][n-1].rgbtBlue;
                counter++;
            }
            if (i - 1 >= 0 && n + 1 < width)
            {
                newred = newred + Original[i-1][n+1].rgbtRed;
                newgreen = newgreen + Original[i-1][n+1].rgbtGreen;
                newblue = newblue + Original[i-1][n+1].rgbtBlue;
                counter++;
            }
            if (i - 1 >= 0 && n - 1 >= 0)
            {
                newred = newred + Original[i-1][n-1].rgbtRed;
                newgreen = newgreen + Original[i-1][n-1].rgbtGreen;
                newblue = newblue + Original[i-1][n-1].rgbtBlue;
                counter++;
            }
            if (i + 1 < height && n + 1 < width)
            {
                newred = newred + Original[i+1][n+1].rgbtRed;
                newgreen = newgreen + Original[i+1][n+1].rgbtGreen;
                newblue = newblue + Original[i+1][n+1].rgbtBlue;
                counter++;
            }
            
    image[i][n].rgbtRed = round(newred / (counter * 1.0));
    image[i][n].rgbtGreen = round(newgreen / (counter * 1.0));
    image[i][n].rgbtBlue = round(newblue / (counter * 1.0));
    
    }

    return;
}
}