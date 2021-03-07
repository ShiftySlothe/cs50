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
    //Black RGB value
    RGBTRIPLE black;
    black.rgbtGreen = 0;
    black.rgbtRed = 0;
    black.rgbtBlue = 0;
    //Allocate memory for temp image[]
    RGBTRIPLE *tempImage;
    tempImage = (RGBTRIPLE *)malloc(sizeof(RGBTRIPLE) * height * width);
    
    RGBTRIPLE gx;
    RGBTRIPLE gy;
    
    //For each row in image
    for (int centreHeightIndex = 0; centreHeightIndex < height; centreHeightIndex++)
    {
        //For each collumn in image
        for (int centreWidthIndex = 0; centreWidthIndex < width; centreWidthIndex++)
        {
            {
                //Set all values to 0
                gx = black;
                gy = black;
                //For the surrounding 3 rows
                for (int currentHeightIndex = centreHeightIndex - 1; currentHeightIndex < centreHeightIndex + 2; currentHeightIndex++)
                {
                    //For the surrounding 3 collums;
                    for (int currentWidthIndex = centreWidthIndex - 1; currentWidthIndex < centreWidthIndex + 2; currentWidthIndex++)
                    {

                        if(currentHeightIndex < height && currentWidthIndex < width && currentHeightIndex >= 0 && currentWidthIndex >= 0)
                        {
                            //3X3 (height, width) grid with (0,0) centre (-1, -1) bottom left, (1, 1) top right
                            switch(centreHeightIndex - currentHeightIndex)
                            {
                                case -1:
                                    switch(currentWidthIndex - centreWidthIndex)
                                    {
                                        //(-1, -1) 
                                        case -1:
                                            gx.rgbtBlue += -image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gx.rgbtRed += -image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gx.rgbtGreen += -image[currentHeightIndex][currentWidthIndex].rgbtGreen;
                                            
                                            gy.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gy.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gy.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen;
                                            break;
                                        //(-1, 0)
                                        case 0:
                                            gy.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue * 2;
                                            gy.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * 2;
                                            gy.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * 2;
                                            break;
                                        //(-1, 1)
                                        case 1:
                                                
                                            gx.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gx.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gx.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen;
                                            
                                            gy.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gy.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gy.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen;;
                                            break;
                                    }
                                    break;
                                case 0:
                                    switch(currentWidthIndex - centreWidthIndex)
                                    {
                                        //(0, -1)
                                        case -1:
                                                gx.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue * -2;
                                                gx.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * -2;
                                                gx.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * -2;                                        
                                            break;
                                        //(0, 0);
                                        case 0:
                                            break;
                                        //(0, 1)        
                                        case 1:
                                                gx.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue * 2;
                                                gx.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * 2;
                                                gx.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * 2;
                                            break;
                                    }
                                    break;
                                case 1:
                                    switch(currentWidthIndex - centreWidthIndex)
                                    {
                                        //(1, -1)
                                        case -1:
                                            gx.rgbtBlue += -image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gx.rgbtRed += -image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gx.rgbtGreen += -image[currentHeightIndex][currentWidthIndex].rgbtGreen;
                                            
                                            gy.rgbtBlue += -image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gy.rgbtRed += -image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gy.rgbtGreen += -image[currentHeightIndex][currentWidthIndex].rgbtGreen;            
                                            break;
                                        //(1, 0)
                                        case 0:
                                            gy.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue * - 2;
                                            gy.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * -2;
                                            gy.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen* -2;    
                                            break;
                                        //(1, 1)
                                        case 1:
                                            gx.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gx.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gx.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen;
                                            
                                            gy.rgbtBlue += -image[currentHeightIndex][currentWidthIndex].rgbtBlue;
                                            gy.rgbtRed += -image[currentHeightIndex][currentWidthIndex].rgbtRed;
                                            gy.rgbtGreen += -image[currentHeightIndex][currentWidthIndex].rgbtGreen;  
                                            break;
                                    }
                                    break;
                            } 
                        }
                    }
                }
                
                //Set the overall values to a float
                float redGX = (float)gx.rgbtRed;
                float blueGX = (float)gx.rgbtBlue;
                float greenGX = (float)gx.rgbtGreen;
                float redGY = (float)gy.rgbtRed;
                float blueGY = (float)gy.rgbtBlue;
                float greenGY = (float)gy.rgbtGreen;
                
                //Combine two values using given formula 
                float redCombinedValue = roundf(sqrtf((powf(redGX, 2)) + powf(redGY, 2)));
                float blueCombinedValue = roundf(sqrtf((powf(blueGX, 2)) + powf(blueGY, 2)));
                float greenCombinedValue = roundf(sqrtf((powf(greenGX, 2)) + powf(greenGY, 2)));
                
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
