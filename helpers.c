#include "helpers.h"
#include "stdlib.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float averageRGB;
    //For each row
    for (int i = 0; i < height; i++)
    {
        //For each collumn
        for (int j = 0; j < width; j++)
        {
            //Calculate average RGB
            averageRGB = roundf(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3);

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
    for (int i = 0; i < height; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width; j++)
        {
            //Set the temp[] to the horizontal mirror pixel
            tempImage[i * width + j] = image[i][width - j - 1];
        }
    }

    //For each row in image
    for (int i = 0; i < height; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width; j++)
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
    for (int i = 0; i < height; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width; j++)
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
            for (int a = heightIndex; a < heightIndex + heightLoopTime; a++)
            {
                //For the surrounding collums;
                for (int b = widthIndex; b < widthIndex + widthLoopTime; b++)
                {
                    pixelCounter++;
                    redRunningTotal += image[a][b].rgbtRed;
                    blueRunningTotal += image[a][b].rgbtBlue;
                    greenRunningTotal += image[a][b].rgbtGreen;
                }
            }

            float redAverage = (redRunningTotal == 0) ? 0 : roundf((float)redRunningTotal / (float)pixelCounter);
            float blueAverage = (blueRunningTotal == 0) ? 0 : roundf((float)blueRunningTotal / (float)pixelCounter);
            float greenAverage = (greenRunningTotal == 0) ? 0 : roundf((float)greenRunningTotal / (float)pixelCounter);

            //Set the temp[].color to the average color valye
            tempImage[i * width + j].rgbtRed = redAverage;
            tempImage[i * width + j].rgbtBlue = blueAverage;
            tempImage[i * width + j].rgbtGreen = greenAverage;
        }
    }

    //For each row in image
    for (int i = 0; i < height; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width; j++)
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
    //Arrays for calculating Gx Gy
    int gxValues[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyValues[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    //Black RGB value
    RGBTRIPLE black;
    black.rgbtGreen = 0;
    black.rgbtRed = 0;
    black.rgbtBlue = 0;
    
    //Allocate memory for temp image[]
    RGBTRIPLE *tempImage;
    tempImage = (RGBTRIPLE *)malloc(sizeof(RGBTRIPLE) * height * width);
    
    //For each row in image
    for (int centreHeightIndex = 0; centreHeightIndex < height; centreHeightIndex++)
    {
        //For each collumn in image
        for (int centreWidthIndex = 0; centreWidthIndex < width; centreWidthIndex++)
        {
            {
                float gxRed = 0;
                float gxGreen = 0;
                float gxBlue = 0;
                
                float gyRed = 0;
                float gyGreen = 0;
                float gyBlue = 0;
                
                //Counter for row loop
                int i = 0;
                //For the surrounding 3 rows
                for (int currentHeightIndex = centreHeightIndex - 1; currentHeightIndex < centreHeightIndex + 2; currentHeightIndex++)
                {
                    //Counter for collum loop
                    int j = 0;
                    //For the surrounding 3 collums;
                    for (int currentWidthIndex = centreWidthIndex - 1; currentWidthIndex < centreWidthIndex + 2; currentWidthIndex++)
                    {
                        if(currentHeightIndex < height && currentWidthIndex < width && currentHeightIndex >= 0 && currentWidthIndex >= 0)
                        {
                            //Calculate Gx
                            gxRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * gxValues[i][j];
                            gxGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * gxValues[i][j];
                            gxBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue *gxValues[i][j];
                            //Calculate Gy
                            gyRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * gyValues[i][j];
                            gyGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * gyValues[i][j];
                            gyBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue * gyValues[i][j];
                        }
                        j++;
                    }
                    i++;
                }
        
                
                //Combine two values using given formula 
                float redCombinedValue = round(sqrt(gxRed * gxRed + gyRed * gyRed));
                float greenCombinedValue = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
                float blueCombinedValue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
                
                redCombinedValue = (redCombinedValue > 255) ? 255 : redCombinedValue;
                blueCombinedValue = (blueCombinedValue > 255) ? 255 : blueCombinedValue;
                greenCombinedValue = (greenCombinedValue > 255) ? 255 : greenCombinedValue;
                
                //Set values to temp array
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtRed = redCombinedValue; 
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtBlue = blueCombinedValue; 
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtGreen = greenCombinedValue; 
            } 
        }
    }

    //For each row in image
    for (int i = 0; i < height; i++)
    {
        //For each collumn in image
        for (int j = 0; j < width; j++)
        {
            //Set the image equal to the temp[]
            image[i][j] = tempImage[i * width + j];
        }
    }
    return;
}
//}}
