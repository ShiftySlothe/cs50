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
    
    RGBTRIPLE gx;
    RGBTRIPLE gy;
    
    //For each row in image
    for (int centreHeightIndex = 0; centreHeightIndex < height; centreHeightIndex++)
    {
        //For each collumn in image
        for (int centreWidthIndex = 0; centreWidthIndex < width; centreWidthIndex++)
        {
            {
                /*
                //Set all values to 0
                gx = black;
                gy = black;
                
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
                            gx.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * gxValues[i][j];
                            gx.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * gxValues[i][j];
                            gx.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue *gxValues[i][j];
                            //Calculate Gy
                            gy.rgbtRed += image[currentHeightIndex][currentWidthIndex].rgbtRed * gyValues[i][j];
                            gy.rgbtGreen += image[currentHeightIndex][currentWidthIndex].rgbtGreen * gyValues[i][j];
                            gy.rgbtBlue += image[currentHeightIndex][currentWidthIndex].rgbtBlue * gyValues[i][j];
                        }
                        j++;
                    }
                    i++;
                }
                //Set the overall values to a float
                float redGX = (float)gx.rgbtRed;
                float greenGX = (float)gx.rgbtGreen;
                float blueGX = (float)gx.rgbtBlue;
                float redGY = (float)gy.rgbtRed;
                float greenGY = (float)gy.rgbtGreen;
                float blueGY = (float)gy.rgbtBlue;
                
                //Combine two values using given formula 
                float redCombinedValue = round(sqrt(redGX * redGX + redGY * redGY));
                float greenCombinedValue = round(sqrt(greenGX * greenGX + greenGY * greenGY));
                float blueCombinedValue = round(sqrt(blueGX * blueGX + blueGY * blueGY));
                
                redCombinedValue = (redCombinedValue > 255) ? 255 : redCombinedValue;
                blueCombinedValue = (blueCombinedValue > 255) ? 255 : blueCombinedValue;
                greenCombinedValue = (greenCombinedValue > 255) ? 255 : greenCombinedValue;
                
                //Set values to temp array
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtRed = redCombinedValue; 
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtBlue = blueCombinedValue; 
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtGreen = greenCombinedValue; 
            }
                */
                
                    // Loop through rows
        for (int i = 0; i < height; i++)
        {
            // Loop through columns
            for (int j = 0; j < width; j++)
            {
                gx = black;
                gy = black;
                // For each pixel, loop vertical and horizontal
                for (int k = -1; k < 2; k++)
                {
                    for (int l = -1; l < 2; l++)
                    {
                        // Check if pixel is outside rows
                        if (i + k < 0 || i + k >= height)
                        {
                            continue;
                        }
                        // Check if pixel is outside columns
                        if (j + l < 0 || j + l >= width)
                        {
                            continue;
                        }
                        // Otherwise add to sums
                        gx.rgbtRed += image[i + k][j + l].rgbtRed * gxValues[k + 1][l + 1];
                        gx.rgbtBlue += image[i + k][j + l].rgbtGreen * gxValues[k + 1][l + 1];
                        gx.rgbtGreen += image[i + k][j + l].rgbtBlue * gxValues[k + 1][l + 1];
                        gy.rgbtRed += image[i + k][j + l].rgbtRed * gyValues[k + 1][l + 1];
                        gy.rgbtGreen += image[i + k][j + l].rgbtGreen * gyValues[k + 1][l + 1];
                        gy.rgbtBlue += image[i + k][j + l].rgbtBlue * gyValues[k + 1][l + 1];
                    }
                }
                
                 //Set the overall values to a float
                float redGX = (float)gx.rgbtRed;
                float greenGX = (float)gx.rgbtGreen;
                float blueGX = (float)gx.rgbtBlue;
                float redGY = (float)gy.rgbtRed;
                float greenGY = (float)gy.rgbtGreen;
                float blueGY = (float)gy.rgbtBlue;
                
                //Combine two values using given formula 
                int redCombinedValue = round(sqrt(redGX * redGX + redGY * redGY));
                int greenCombinedValue = round(sqrt(greenGX * greenGX + greenGY * greenGY));
                int blueCombinedValue = round(sqrt(blueGX * blueGX + blueGY * blueGY));
                
                redCombinedValue = (redCombinedValue > 255) ? 255 : redCombinedValue;
                blueCombinedValue = (blueCombinedValue > 255) ? 255 : blueCombinedValue;
                greenCombinedValue = (greenCombinedValue > 255) ? 255 : greenCombinedValue;
                
                //Set values to temp array
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtRed = redCombinedValue; 
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtBlue = blueCombinedValue; 
                tempImage[centreHeightIndex * width + centreWidthIndex].rgbtGreen = greenCombinedValue; 
                  /*          // Calculate Sobel operator
                int red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
                int green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
                int blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));
                // Cap at 255
                if (red > 255)
                {
                    red = 255;
                }
                if (green > 255)
                {
                    green = 255;
                }
                if (blue > 255)
                {
                    blue = 255;
                }
                
                // Assign new values to pixels
                tempImage[i* width + j].rgbtRed = red;
                tempImage[i* width + j].rgbtGreen = green;
                tempImage[i* width + j].rgbtBlue = blue; */
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
}}}
