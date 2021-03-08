#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t Byte;

int main(int argc, char *argv[])
{
    //Check CLA
    if (argc != 2)
    {
        printf("Useage: 1 raw file required.");
        return 1;
    }
    
    //Open file and ensure we have a pointer
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile  == NULL)
    {
        printf("Cannot read file, exiting.");
        return 1;
    }
    
    //Create buffer to store block
    Byte buffer[512];
    
    int numberOfJPEG = 0;
    char currentFileName[8];
    FILE *img = NULL;
    bool jpegFound = false;
    
    //Read the file to buffer
    while (fread(&buffer, 512, 1, inputFile) == 1)
    {
        //If we find a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //And it's not the first
            if (numberOfJPEG != 0)
            {
                fclose(img);
            }
            //Set file name
            sprintf(currentFileName, "%03i.jpg", numberOfJPEG);
            //Open file
            img = fopen(currentFileName, "w");
            numberOfJPEG++;
            jpegFound = true;
        }
        
        if (jpegFound)
        {
            fwrite(buffer, 512, 1, img);
        }
    }
    fclose(img);
}