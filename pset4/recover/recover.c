#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    //check to be sure command line argument is one file
    if (argc != 2)
    {
        printf("Please provide one file name.");
        return 1;
    }

    //open file for reading and check to be sure it exists
    FILE *recoveryfile = fopen(argv[1], "r");
    if (recoveryfile == NULL)
    {
        printf("File does not exist or is not ccompatible");
        return 1;
    }

    //create a pointer to a file where the images will be stored, set it to nothing
    FILE *images = NULL;

    //create a buffer to go through file 512 bytes at a time
    unsigned char buffer[512];

    //create another array to store the file names, 8 digits because the file name is 3 plus .jpg plus end marker
    char filename[8];

    //set a counter to keep track of filename
    int counter = 0;

    //set a bool flag to keep track of if the file is open or closed
    bool flag = false;

    //read through the file at 512 bytes at a time, until there are no more bytes
    while (fread(buffer, 512, 1, recoveryfile) == 1)
    {
        //check to see if the buffer is reading a JPEG by looking at the first four bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] &0xf0) == 0xe0)
        {
            //if a file is already open, close it so we can write a different one
            if (flag == true)
            {
                fclose(images);
            }
            //open file for found JPEG
            else
            {
                flag = true;
            }

            //begin to print image in file
            sprintf(filename,"%.03i.jpg", counter);
            images = fopen(filename, "w");
            counter++;
        }

        //if flag is true
        if (flag == true)
        {
            fwrite(&buffer, 512, 1, images);
        }
    }

    //close all files
    fclose(recoveryfile);
    fclose(images);

    return 0;
}
