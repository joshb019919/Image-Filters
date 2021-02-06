#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Collect average as float to satisfy check50 expectations.
    float rgb_ave;

    // Loop through pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Collect average.
            rgb_ave = (image[i][j].rgbtBlue +
                       image[i][j].rgbtGreen +
                       image[i][j].rgbtRed) / 3.0;

            // Round average.
            rgb_ave = round(rgb_ave);

            // Set each color to average; gray.
            image[i][j].rgbtBlue = rgb_ave;
            image[i][j].rgbtGreen = rgb_ave;
            image[i][j].rgbtRed = rgb_ave;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Swapping temp.
    RGBTRIPLE tmp_pxl;

    // Loop through pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width / 2); j++)
        {
            // Swap pixels up till half to keep from reswapping.
            tmp_pxl = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp_pxl;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Found pseudo-code on a box blur wiki article; adapted it.
    // The reason to make a copy of all pixels is because, otherwise,
    // the changed pixel will be part of the new calculation.

    // Image copy, to keep new, blurred pixel from being used.
    RGBTRIPLE image_copy[height][width];
    // Blue, green, and red totals, and color counter.
    float b_t = 0.0, g_t = 0.0, r_t = 0.0, rgbs = 0.0;

    // Loop through pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Loop through box.
            for (int k = -1; k < 2; k++)
            {
                for (int m = -1; m < 2; m++)
                {
                    // Skip if out of bounds.
                    if ((i + k < 0 && j + m < 0) ||
                        i + k < 0 || i + k == height ||
                        j + m < 0 || j + m == width ||
                        (i + k == height && j + m == width))
                    {
                        continue;
                    }

                    // Else increment r's, g's, b's, and divisor.
                    b_t += image[i + k][j + m].rgbtBlue;
                    g_t += image[i + k][j + m].rgbtGreen;
                    r_t += image[i + k][j + m].rgbtRed;
                    rgbs++;
                }
            }

            // Add new, blurred pixel to image copy.
            image_copy[i][j].rgbtBlue = round(b_t / rgbs);
            image_copy[i][j].rgbtGreen = round(g_t / rgbs);
            image_copy[i][j].rgbtRed = round(r_t / rgbs);
            // Reset counts.
            b_t = 0.0, g_t = 0.0, r_t = 0.0, rgbs = 0.0;
        }
    }

    // Copy pixels from image copy to original.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy of image to keep from calculating off changed pixels.
    RGBTRIPLE image_copy[height][width];
    // Sobel's multipliers.
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Sum total of each channel for each, x and y.
    float bx_t = 0.0, gx_t = 0.0, rx_t = 0.0;
    float by_t = 0.0, gy_t = 0.0, ry_t = 0.0;
    // Squared sqrt'ed, and rounded values.
    int b_t, g_t, r_t;

    // Loop through 2d image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Loop through box.
            for (int k = -1; k < 2; k++)
            {
                for (int m = -1; m < 2; m++)
                {
                    // Skip if out of bounds.
                    if ((i + k < 0 && j + m < 0) ||
                        i + k < 0 || i + k == height ||
                        j + m < 0 || j + m == width ||
                        (i + k == height && j + m == width))
                    {
                        continue;
                    }

                    bx_t += image[i + k][j + m].rgbtBlue * Gx[k + 1][m + 1];
                    gx_t += image[i + k][j + m].rgbtGreen * Gx[k + 1][m + 1];
                    rx_t += image[i + k][j + m].rgbtRed * Gx[k + 1][m + 1];
                    by_t += image[i + k][j + m].rgbtBlue * Gy[k + 1][m + 1];
                    gy_t += image[i + k][j + m].rgbtGreen * Gy[k + 1][m + 1];
                    ry_t += image[i + k][j + m].rgbtRed * Gy[k + 1][m + 1];
                }
            }
            
            // Blue, green, and red totals.
            b_t = round(sqrt(pow(bx_t, 2) + pow(by_t, 2)));
            g_t = round(sqrt(pow(gx_t, 2) + pow(gy_t, 2)));
            r_t = round(sqrt(pow(rx_t, 2) + pow(ry_t, 2)));
            
            // Assign to image's copy.
            image_copy[i][j].rgbtBlue = (b_t < 255) ? b_t : 255;
            image_copy[i][j].rgbtGreen = (g_t < 255) ? g_t : 255;
            image_copy[i][j].rgbtRed = (r_t < 255) ? r_t : 255;
            
            // Reset incrementals.
            bx_t = 0.0, gx_t = 0.0, rx_t = 0.0;
            by_t = 0.0, gy_t = 0.0, ry_t = 0.0;
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
}
