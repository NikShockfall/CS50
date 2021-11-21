#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

void add(int *blue,int *green,int *red, RGBTRIPLE pixel);
void convolute(int factor, int *blue,int *green,int *red, RGBTRIPLE pixel);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = width - 1; j < width / 2; j++, k--)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blur[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blue = 0;
            int green = 0;
            int red = 0;
            float count = 0.0;



            for (int k = i-1; k <= i+1 && k < height; k++)
            {
                if (k < 0)
                {
                    k++;
                }

                for (int l = j-1; l <= j+1 && l < width; l++)
                {
                    if (l < 0)
                    {
                        l++;
                    }

                    add(&blue, &green, &red, image[k][l]);
                    count += 1;
                }
            }

            blur[i][j].rgbtBlue = round(blue / count);
            blur[i][j].rgbtGreen = round(green / count);
            blur[i][j].rgbtRed = round(red / count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blur[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int colors[2][3] = {{0, 0, 0},
                            {0, 0, 0}};

            if (i > 0)
            {
                if (j > 0)
                {
                    // Upper left
                    convolute(-1, &colors[0][0], &colors[0][1], &colors[0][2], image[i-1][j-1]);
                    convolute(-1, &colors[1][0], &colors[1][1], &colors[1][2], image[i-1][j-1]);
                }

                // Upper center
                convolute(-2, &colors[1][0], &colors[1][1], &colors[1][2], image[i-1][j]);

                if (j < width - 1)
                {
                    //Upper right
                    convolute(1, &colors[0][0], &colors[0][1], &colors[0][2], image[i-1][j+1]);
                    convolute(-1, &colors[1][0], &colors[1][1], &colors[1][2], image[i-1][j+1]);
                }
            }

            if (j > 0)
            {
                // Left
                convolute(-2, &colors[0][0], &colors[0][1], &colors[0][2], image[i][j-1]);
            }

            if (j < width - 1)
            {
                //Right
                convolute(2, &colors[0][0], &colors[0][1], &colors[0][2], image[i][j+1]);
            }


            if (i < height - 1)
            {
                if (j > 0)
                {
                    // Lower left
                    convolute(-1, &colors[0][0], &colors[0][1], &colors[0][2], image[i+1][j-1]);
                    convolute(1, &colors[1][0], &colors[1][1], &colors[1][2], image[i+1][j-1]);
                }

                //Lower center
                convolute(2, &colors[1][0], &colors[1][1], &colors[1][2], image[i+1][j]);

                if (j < width - 1)
                {
                    //Lower right
                    convolute(1, &colors[0][0], &colors[0][1], &colors[0][2], image[i+1][j+1]);
                    convolute(1, &colors[1][0], &colors[1][1], &colors[1][2], image[i+1][j+1]);
                }


            }

            colors[0][0] = round(sqrt(pow(colors[0][0], 2.0) + pow(colors[1][0], 2.0)));
            colors[0][1] = round(sqrt(pow(colors[0][1], 2.0) + pow(colors[1][1], 2.0)));
            colors[0][2] = round(sqrt(pow(colors[0][2], 2.0) + pow(colors[1][2], 2.0)));
            edge[i][j].rgbtBlue = colors[0][0] > 255 ? 255 : colors[0][0];
            edge[i][j].rgbtGreen = colors[0][1] > 255 ? 255 : colors[0][1];
            edge[i][j].rgbtRed = colors[0][2] > 255 ? 255 : colors[0][2];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edge[i][j];
        }
    }
}



void add(int *blue,int *green,int *red, RGBTRIPLE pixel)
{
    *blue += pixel.rgbtBlue;
    *green += pixel.rgbtGreen;
    *red += pixel.rgbtRed;
}

void convolute(int factor, int *blue,int *green,int *red, RGBTRIPLE pixel)
{
    int color[3]  = {pixel.rgbtBlue, pixel.rgbtGreen, pixel.rgbtRed};

    *blue += color[0] * factor;
    *green += color[1] * factor;
    *red += color[2] * factor;


}