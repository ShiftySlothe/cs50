#include "helpers.h"
#include "stdlib.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int averageRGB;
    //For each row
    for (int i = 0; i < height - 1; i++)
    {
        //For each collumn 
        for (int j = 0; j < width - 1; j++)
        {
            //Calculate average RGB
            averageRGB = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            
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
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
