/* Script to apply filters to bitmaps that have been read in byte by byte into the array image */

#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int max(int a, int b);
int min(int a, int b);

// Convert image to grayscale by setting the RGB values to the average of the three.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    printf("%i, %i, %i, %i\n", image[0][0].rgbtRed, image[0][1].rgbtRed, image[1][0].rgbtRed, image[1][1].rgbtRed);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            BYTE average = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j=0; j < width/2; j++){
            BYTE tmpred = image[i][j].rgbtRed;
            BYTE tmpblue = image[i][j].rgbtBlue;
            BYTE tmpgreen = image[i][j].rgbtGreen;

            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;

            image[i][width - 1 - j].rgbtRed = tmpred;
            image[i][width - 1 - j].rgbtBlue = tmpblue;
            image[i][width - 1 - j].rgbtGreen = tmpgreen;
        }
    }

    return;
}

// Blur image by setting the RGB values to the average of the surrounding pixels.
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE blurred[height][width];
    for (int i = 0; i < height; i++){
        for (int j=0; j < width; j++){
            int startx = max(j-1, 0);
            int endx = min(j+1, width-1);
            int starty = max(i-1, 0);
            int endy = min(i+1, height-1);

            float counter = 0.0;
            int avgred = 0, avgblue = 0, avggreen = 0;

            for (int k = 0; k <= (endy-starty); k++){
                for (int l = 0; l <= (endx-startx); l++){
                    counter += 1;
                    avgred += image[starty+k][startx+l].rgbtRed;
                    avgblue += image[starty+k][startx+l].rgbtBlue;
                    avggreen += image[starty+k][startx+l].rgbtGreen;
                }
            }



            blurred[i][j].rgbtRed = (int) round(avgred/counter);
            blurred[i][j].rgbtBlue = (int) round(avgblue/counter);
            blurred[i][j].rgbtGreen = (int) round(avggreen/counter);

        }
    }


    for (int i = 0; i < height; i++){
        for (int j=0; j < width; j++){
            image[i][j]=blurred[i][j];
        }
    }

    return;
}


// 
void sepia(int height, int width, RGBTRIPLE image[height][width]){
    for (int i = 0; i < height; i++){
        for (int j=0; j < width; j++){
            image[i][j].rgbtRed = min(255, round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtGreen));
            image[i][j].rgbtRed = min(255, round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtGreen));
            image[i][j].rgbtRed = min(255, round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtGreen));
        }
    }
    return;
}

bool valid(int k, int l, int w, int h){
    return k >= 0 && k < h && l >= 0 && l < w;
}

// Edge detection filter
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    /* Define two 3x3 kernels that will return 0 when multiplied with sections of the image if RGB values are 
    same and will return a non zero value if not i.e. detecting edges */
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE edge[height][width];

    for (int i = 0; i < height; i++){
        for (int j=0; j < width; j++){
            //find 3x3 surronding matrix for each pixel
            RGBTRIPLE surr[3][3];

            for (int k = 0; k <= 2; k++){
                for (int l = 0; l<=2; l++){
                    if (valid(i+k-1, j+l-1, width, height)){
                        surr[2-k][l] = image[i+k-1][j+l-1];
                    }
                    else{
                        surr[2-k][l].rgbtRed = 0;
                        surr[2-k][l].rgbtBlue = 0;
                        surr[2-k][l].rgbtGreen = 0;
                    }
                }
            }
            
            //calculate product of surronding matrix and kernel
            int xRedWSum = 0, xBlueWSum = 0, xGreenWSum = 0;
            int yRedWSum = 0, yBlueWSum = 0, yGreenWSum = 0;
            for (int k = 0; k <= 2; k++){
                for (int l = 0; l<=2; l++){
                    xRedWSum += surr[k][l].rgbtRed * Gx[k][l];
                    yRedWSum += surr[k][l].rgbtRed * Gy[k][l];
                    xBlueWSum += surr[k][l].rgbtBlue * Gx[k][l];
                    yBlueWSum += surr[k][l].rgbtBlue * Gy[k][l];
                    xGreenWSum += surr[k][l].rgbtGreen * Gx[k][l];
                    yGreenWSum += surr[k][l].rgbtGreen * Gy[k][l];
                }
            }

            //Find L^2 norm of xtotal and ytotal
            double finalRedvalue = sqrt(xRedWSum*xRedWSum + yRedWSum*yRedWSum);
            double finalBluevalue = sqrt(xBlueWSum*xBlueWSum + yBlueWSum*yBlueWSum);
            double finalGreenvalue = sqrt(xGreenWSum*xGreenWSum + yGreenWSum*yGreenWSum);
            
            // Ensure value is an integer and not greater than 255
            edge[i][j].rgbtRed = min(round(finalRedvalue), 255);
            edge[i][j].rgbtBlue = min(round(finalBluevalue), 255);
            edge[i][j].rgbtGreen = min(round(finalGreenvalue), 255);

        }
    }
    
    // overwrite image with temporary image
    for (int i = 0; i < height; i++){
        for (int j=0; j < width; j++){
            image[i][j]=edge[i][j];
        }
    }

    return;


    return;
}

// Find maximum of two numbers
int max(int a, int b){
    if (a >= b){
        return a;
    }
    else{
        return b;
    }
}

// Find minimum of two numbers
int min(int a, int b){
    if (a >= b){
        return b;
    }
    else{
        return a;
    }
}
