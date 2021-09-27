#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float gray_factor;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gray_factor = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = gray_factor;
            image[i][j].rgbtGreen = gray_factor;
            image[i][j].rgbtRed = gray_factor;

        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    double sepia_factorBlue;
    double sepia_factorGreen;
    double sepia_factorRed;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepia_factorBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue));
            if (sepia_factorBlue > 255)
            {
                sepia_factorBlue = 255;
            }
            sepia_factorGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue));
            if (sepia_factorGreen > 255)
            {
                sepia_factorGreen = 255;
            }
            sepia_factorRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue));
            if (sepia_factorRed > 255)
            {
                sepia_factorRed = 255;
            }
            image[i][j].rgbtBlue = sepia_factorBlue;
            image[i][j].rgbtGreen = sepia_factorGreen;
            image[i][j].rgbtRed = sepia_factorRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE placeholder[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width - 1; j < (width / 2); j++, n--)
        {
            placeholder[i][j] = image[i][j];
            image[i][j] = image[i][n];
            image[i][n] = placeholder[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];
    float red_Avg;
    float blue_Avg;
    float green_Avg;
    int blur_factor;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // If number is not on the edges
            if (i > 0 && j > 0 && i < height - 1 && j < width - 1)
            {
                // Sum rows
                blue_Avg = image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue
                           + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                           + image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue;

                green_Avg = image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen
                            + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                            + image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen;

                red_Avg = image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed
                          + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed
                          + image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed;

                // Calculate blur factor
                blur[i][j].rgbtBlue = round((float)blue_Avg / 9.0);
                blur[i][j].rgbtGreen = round((float)green_Avg / 9.0);
                blur[i][j].rgbtRed = round((float)red_Avg / 9.0);

            }
            // If it is on the top left corner
            else if (i == 0 && j == 0)
            {
                // Sum rows
                blue_Avg = image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i +  1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                green_Avg = image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i +  1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                red_Avg = image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i +  1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 4.0);
                blur[i][j].rgbtGreen = round(green_Avg / 4.0);
                blur[i][j].rgbtRed = round(red_Avg / 4.0);
            }
            // If it is on the top right corner
            else if (i == 0 && j == width - 1)
            {
                // Sum itself with its neighbour
                blue_Avg = image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i +  1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue;
                green_Avg = image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i +  1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen;
                red_Avg = image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i +  1][j].rgbtRed + image[i + 1][j - 1].rgbtRed;

                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 4.0);
                blur[i][j].rgbtGreen = round(green_Avg / 4.0);
                blur[i][j].rgbtRed = round(red_Avg / 4.0);
            }
            // If it is on the top edge, not in the corner
            else if (i == 0 && j > 0 && j < width - 1)
            {
                // Sum neighbours
                blue_Avg = image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue
                           + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;

                green_Avg = image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen
                            + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

                red_Avg = image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j + 1].rgbtRed
                          + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 6.0);
                blur[i][j].rgbtGreen = round(green_Avg / 6.0);
                blur[i][j].rgbtRed = round(red_Avg / 6.0);
            }
            // If it is on the below left corner
            else if (i == height - 1 && j == 0)
            {
                // Sum itself with its neighbour
                blue_Avg = image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i -  1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue;
                green_Avg = image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +  image[i -  1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen;
                red_Avg = image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i -  1][j].rgbtRed + image[i - 1][j + 1].rgbtRed;
                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 4.0);
                blur[i][j].rgbtGreen = round(green_Avg / 4.0);
                blur[i][j].rgbtRed = round(red_Avg / 4.0);
            }
            // If it is on the below right corner
            else if (i == height - 1 && j == width - 1)
            {
                // Sum itself with its neighbour
                blue_Avg = image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i -  1][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue;
                green_Avg = image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i -  1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen;
                red_Avg = image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i -  1][j].rgbtRed + image[i - 1][j - 1].rgbtRed;
                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 4.0);
                blur[i][j].rgbtGreen = round(green_Avg / 4.0);
                blur[i][j].rgbtRed = round(red_Avg / 4.0);
            }
            // If it is on the below edge, not in the corner
            else if (i == height - 1 && j > 0 && j < width - 1)
            {
                // Sum neighbours
                blue_Avg = image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue
                           + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue;

                green_Avg = image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen
                            + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen;

                red_Avg = image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j + 1].rgbtRed
                          + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed;
                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 6.0);
                blur[i][j].rgbtGreen = round(green_Avg / 6.0);
                blur[i][j].rgbtRed = round(red_Avg / 6.0);
            }
            // If it is on the left edge, not in the corner
            else if (j == 0 && i > 0 && i < height - 1)
            {
                // Sum neighbours
                blue_Avg = image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue
                           + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;


                green_Avg = image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen
                            + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

                red_Avg = image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed
                          + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;

                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 6.0);
                blur[i][j].rgbtGreen = round(green_Avg / 6.0);
                blur[i][j].rgbtRed = round(red_Avg / 6.0);
            }
            // If it is on the right edge, not in the corner
            else if (j == width - 1 && i > 0 && i < height - 1)
            {
                // Sum neighbours
                blue_Avg = image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue
                           + image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue;


                green_Avg = image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen
                            + image[i + 1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen;

                red_Avg = image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j - 1].rgbtRed
                          + image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed;

                // Calculate blur factor
                blur[i][j].rgbtBlue = round(blue_Avg / 6.0);
                blur[i][j].rgbtGreen = round(green_Avg / 6.0);
                blur[i][j].rgbtRed = round(red_Avg / 6.0);
            }
        }

    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
        }
    }
}