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
    
    //Try seek end of file
    if (fseek(inputFile, 0L, SEEK_END) != 0)
    {
        printf("File read error, exiting");
        return 1;
    }
    //Create buffer to store block
    Byte buffer[512];
    
    int numberOfJPEG = -1;
    char *currentFileName = "000";
    bool jpegFound = false;
    FILE *img = fopen(currentFileName, "w");
    //Read the file to buffer
    while (fread(buffer, 512, 1, inputFile) == 512)
    {
        //Gets only the first 4 of the 4th buffer, setting last to 0
        Byte firstFourFourthBuffer = buffer[3] & 0xf0;
        
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && firstFourFourthBuffer == 0xe0)
        {
            jpegFound = true;
            numberOfJPEG++;
            if(numberOfJPEG == 0)
            {
                continue;
            }
            else
            {
                fclose(img);
                sprintf(currentFileName, "%03i", numberOfJPEG);
                img = fopen(currentFileName, "w");
            }
        }
        if(jpegFound)
            fwrite(buffer, 512, 1, img);
    }
    fclose(img);
}