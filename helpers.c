#include "helpers.h"
#include "stdlib.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float averageRGB;
    //For each row
    for (int i = 0; i < height - 1; i++)
    {
        //For each collumn 
        for (int j = 0; j < width - 1; j++)
        {
            //Calculate average RGB
            averageRGB = round(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            
            //Set each pixel to the average
            image[i][j].rgbtBlue = averageRGB;
            image[i][j].rgbtGreen = averageRGB;
            image[i][j].rgbtRed = averageRGB;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Allocate memory for temp image[]
    RGBTRIPLE *tempImage;
    tempImage = (RGBTRIPLE *)malloc(sizeof(RGBTRIPLE) * height * width);
    
    //For each row in image
    for (int i = 0; i < height - 1; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width - 1; j++)
        {
            //Set the temp[] to the horizontal mirror pixel 
            tempImage[i * width + j] = image[i][width - j];
        }
    }
    
    //For each row in image
    for (int i = 0; i < height - 1; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width - 1; j++)
        {
            //Set the image equal  to the temp array
            image[i][j] = tempImage[i * width + j]; 
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Allocate memory for temp image[]
    RGBTRIPLE *tempImage;
tempImage = (RGBTRIPLE *)malloc(sizeof(RGBTRIPLE) * height * width);
    
    //For each row in image
    for (int i = 0; i < height - 1; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width - 1; j++)
        {
            //Number of pixels used to calculate average
            int pixelCounter = 0;
        
            int redRunningTotal = 0;
            int blueRunningTotal = 0;
            int greenRunningTotal = 0;
            
            //If on left or top edge, will start loop on same row/collum as edge.
            int heightIndex = (i == 0) ? i : i - 1;
            int widthIndex = (j == 0) ? j : j - 1;
            
            //If on any edge will loop twice, else 3 times
            int heightLoopTime = (i == 0) ? 2 : (i == height - 1) ? 2 : 3;
            int widthLoopTime = (j == 0) ? 2 : (j == width - 1) ? 2 : 3;
            
            //For the surrounding rows
            for (int a = heightIndex; a < heightLoopTime; a++)
            {
                //For the surrounding collums;
                for (int b = widthIndex; b < widthLoopTime; b++)
                {
                    pixelCounter++;
                    redRunningTotal += image[a][b].rgbtRed;
                    blueRunningTotal += image[a][b].rgbtBlue;
                    greenRunningTotal += image[a][b].rgbtGreen;
                }
            }
            
            //Calculate average color value
            redRunningTotal = (redRunningTotal == 0) ? 0 : redRunningTotal / pixelCounter;
            blueRunningTotal = (blueRunningTotal == 0) ? 0 : blueRunningTotal / pixelCounter;
            greenRunningTotal = (greenRunningTotal == 0) ? 0 : greenRunningTotal / pixelCounter;
            
            //Set the temp[].color to the average color valye
            tempImage[i * width + j].rgbtRed = redRunningTotal;
            tempImage[i * width + j].rgbtBlue = blueRunningTotal;
            tempImage[i * width + j].rgbtGreen = greenRunningTotal;
        }
    }
    
    //For each row in image
    for (int i = 0; i < height - 1; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width - 1; j++)
        {
            //Set the image equal to the temp[]
            image[i][j] = tempImage[i * width + j]; 
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
