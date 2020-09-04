#include <stdio.h>
#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{   
    // find average color
    int average_color;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset average_color
            average_color = 0;
            RGBTRIPLE pixel = image[i][j];
            average_color +=  round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / (float) 3);
            // set every value to average_color
            image[i][j].rgbtRed = average_color;
            image[i][j].rgbtGreen = average_color;
            image[i][j].rgbtBlue = average_color;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int count = round(width / 2);
    // Preserve width
    int width_copy = width;

    for (int i = 0; i < height; i++)
    {
        // Reset width
        width = width_copy - 1;

        for (int j = 0; j < count; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width];
            image[i][width] = temp;
            width--;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int average_color_red, average_color_green, average_color_blue, counter;
    // Create copy of original colors
    RGBTRIPLE org_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            org_image[i][j] = image[i][j];
        }
    }
    // Average color
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reset pixel counter and colors
            counter = 0;
            average_color_red = average_color_green = average_color_blue = 0;

            // Add current pixel
            average_color_red += org_image[i][j].rgbtRed;
            average_color_green += org_image[i][j].rgbtGreen;
            average_color_blue += org_image[i][j].rgbtBlue;
            counter++;
            
            // Add left and right pixel
            if (j >= 0 && j <= width - 1)
            {
                // Left
                if (j > 0)
                {
                    average_color_red += org_image[i][j - 1].rgbtRed;
                    average_color_green += org_image[i][j - 1].rgbtGreen;
                    average_color_blue += org_image[i][j - 1].rgbtBlue;
                    counter++;
                }
                
                // Right
                if (j < width - 1)
                {
                    average_color_red += org_image[i][j + 1].rgbtRed;
                    average_color_green += org_image[i][j + 1].rgbtGreen;
                    average_color_blue += org_image[i][j + 1].rgbtBlue;
                    counter++;
                }
            }
            
            // Add top and bottom
            if (i >= 0 && i <= height - 1)
            {
                // Top
                if (i > 0)
                {
                    average_color_red += org_image[i - 1][j].rgbtRed;
                    average_color_green += org_image[i - 1][j].rgbtGreen;
                    average_color_blue += org_image[i - 1][j].rgbtBlue;
                    counter++;
                }

                // Bottom
                if (i < height - 1)
                {
                    average_color_red += org_image[i + 1][j].rgbtRed;
                    average_color_green += org_image[i + 1][j].rgbtGreen;
                    average_color_blue += org_image[i + 1][j].rgbtBlue;
                    counter++;
                }
            }
            
            // Add upper-left and bottom-left
            if ((i >= 0 && i <= height - 1) && (j > 0))
            {
                // Upper-left
                if (i > 0)
                {
                    average_color_red += org_image[i - 1][j - 1].rgbtRed;
                    average_color_green += org_image[i - 1][j - 1].rgbtGreen;
                    average_color_blue += org_image[i - 1][j - 1].rgbtBlue;
                    counter++;
                }

                // Bottom-left
                if (i < height - 1)
                {
                    average_color_red += org_image[i + 1][j - 1].rgbtRed;
                    average_color_green += org_image[i + 1][j - 1].rgbtGreen;
                    average_color_blue += org_image[i + 1][j - 1].rgbtBlue;
                    counter++;
                }
            }

            // Add upper-right and bottom-right
            if ((i >= 0 && i <= height - 1) && (j < width - 1))
            {
                // Upper-right
                if (i > 0)
                {
                    average_color_red += org_image[i - 1][j + 1].rgbtRed;
                    average_color_green += org_image[i - 1][j + 1].rgbtGreen;
                    average_color_blue += org_image[i - 1][j + 1].rgbtBlue;
                    counter++;
                }
                
                // Bottom-right
                if (i < height - 1)
                {
                    average_color_red += org_image[i + 1][j + 1].rgbtRed;
                    average_color_green += org_image[i + 1][j + 1].rgbtGreen;
                    average_color_blue += org_image[i + 1][j + 1].rgbtBlue;
                    counter++;
                }
            }

            // Calculate averages
            average_color_red = round(average_color_red / (float) counter);
            average_color_green = round(average_color_green / (float) counter);
            average_color_blue = round(average_color_blue / (float) counter);
            
            // Change color values of pixel
            image[i][j].rgbtRed = average_color_red;
            image[i][j].rgbtGreen = average_color_green;
            image[i][j].rgbtBlue = average_color_blue;

        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of original colors
    RGBTRIPLE org_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            org_image[i][j] = image[i][j];
        }
    }

    // Channels
    float r_Gx, g_Gx, b_Gx, r_Gy, g_Gy, b_Gy;
    int all_channels[3];

    // Calculate weighted sums
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reset colors
            r_Gx = g_Gx = b_Gx = r_Gy = g_Gy = b_Gy = 0;

            // Add left and right pixel
            if (j >= 0 && j <= width - 1)
            {
                // Left
                if (j > 0)
                {
                    r_Gx += (-2) * org_image[i][j - 1].rgbtRed;
                    g_Gx += (-2) * org_image[i][j - 1].rgbtGreen;
                    b_Gx += (-2) * org_image[i][j - 1].rgbtBlue;
                }
                
                // Right
                if (j < width - 1)
                {
                    r_Gx += (2) * org_image[i][j + 1].rgbtRed;
                    g_Gx += (2) * org_image[i][j + 1].rgbtGreen;
                    b_Gx += (2) * org_image[i][j + 1].rgbtBlue;
                }
            }
            
            // Add top and bottom
            if (i >= 0 && i <= height - 1)
            {
                // Top
                if (i > 0)
                {
                    r_Gy += (-2) * org_image[i - 1][j].rgbtRed;
                    g_Gy += (-2) * org_image[i - 1][j].rgbtGreen;
                    b_Gy += (-2) * org_image[i - 1][j].rgbtBlue;
                }

                // Bottom
                if (i < height - 1)
                {
                    r_Gy += (2) * org_image[i + 1][j].rgbtRed;
                    g_Gy += (2) * org_image[i + 1][j].rgbtGreen;
                    b_Gy += (2) * org_image[i + 1][j].rgbtBlue;
                }
            }
            
            // Add upper-left and bottom-left
            if ((i >= 0 && i <= height - 1) && (j > 0))
            {
                // Upper-left
                if (i > 0)
                {
                    r_Gx += (-1) * org_image[i - 1][j - 1].rgbtRed;
                    g_Gx += (-1) * org_image[i - 1][j - 1].rgbtGreen;
                    b_Gx += (-1) * org_image[i - 1][j - 1].rgbtBlue;

                    r_Gy += (-1) * org_image[i - 1][j - 1].rgbtRed;
                    g_Gy += (-1) * org_image[i - 1][j - 1].rgbtGreen;
                    b_Gy += (-1) * org_image[i - 1][j - 1].rgbtBlue;
                }

                // Bottom-left
                if (i < height - 1)
                {
                    r_Gx += (-1) * org_image[i + 1][j - 1].rgbtRed;
                    g_Gx += (-1) * org_image[i + 1][j - 1].rgbtGreen;
                    b_Gx += (-1) * org_image[i + 1][j - 1].rgbtBlue;

                    r_Gy += (1) * org_image[i + 1][j - 1].rgbtRed;
                    g_Gy += (1) * org_image[i + 1][j - 1].rgbtGreen;
                    b_Gy += (1) * org_image[i + 1][j - 1].rgbtBlue;
                }
            }

            // Add upper-right and bottom-right
            if ((i >= 0 && i <= height - 1) && (j < width - 1))
            {
                // Upper-right
                if (i > 0)
                {
                    r_Gx += (1) * org_image[i - 1][j + 1].rgbtRed;
                    g_Gx += (1) * org_image[i - 1][j + 1].rgbtGreen;
                    b_Gx += (1) * org_image[i - 1][j + 1].rgbtBlue;

                    r_Gy += (-1) * org_image[i - 1][j + 1].rgbtRed;
                    g_Gy += (-1) * org_image[i - 1][j + 1].rgbtGreen;
                    b_Gy += (-1) * org_image[i - 1][j + 1].rgbtBlue;
                }
                
                // Bottom-right
                if (i < height - 1)
                {
                    r_Gx += (1) * org_image[i + 1][j + 1].rgbtRed;
                    g_Gx += (1) * org_image[i + 1][j + 1].rgbtGreen;
                    b_Gx += (1) * org_image[i + 1][j + 1].rgbtBlue;

                    r_Gy += (1) * org_image[i + 1][j + 1].rgbtRed;
                    g_Gy += (1) * org_image[i + 1][j + 1].rgbtGreen;
                    b_Gy += (1) * org_image[i + 1][j + 1].rgbtBlue;
                }
            }

            // Combine Gx and Gy
            int red_channel = round(sqrt((r_Gx * r_Gx) + (r_Gy * r_Gy)));
            all_channels[0] = red_channel;
            int green_channel = round(sqrt((g_Gx * g_Gx) + (g_Gy * g_Gy)));
            all_channels[1] = green_channel;
            int blue_channel = round(sqrt((b_Gx * b_Gx) + (b_Gy * b_Gy)));
            all_channels[2] = blue_channel;
            // printf(" Red Channel:\n>> %i\n", red_channel);
            // printf("Green Channel:\n>> %i\n", green_channel);
            // printf("Blue Channel:\n>> %i\n", blue_channel);

            // Check if goes out of allowed values (0 - 255)
            for (int n = 0; n < 3; n++)
            {
                if (all_channels[n] > 255)
                {
                    all_channels[n] = 255;
                }
            }
            
            // Change color values of pixel
            image[i][j].rgbtRed = all_channels[0];
            image[i][j].rgbtGreen = all_channels[1];
            image[i][j].rgbtBlue = all_channels[2];

        }
    }
}
