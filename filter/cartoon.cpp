//
//  cartoon.cpp
//  filter
//
//  Created by Дарья Кабанова on 07.06.13.
//  Copyright (c) 2013 Дарья Кабанова. All rights reserved.
//

#include "cartoon.h"
#include <math.h>
#define COUNT_OF_COLORS     255
#define COUNT_OF_CHANNELS   3

        
float ** Cartoon::GaussianFunction(float sigma,int radius)
{
    printf("\n");
    int size=2*radius+1;
    float coeff = 1.0f/(2.0f*(float)M_PI*sigma*sigma);
    float ** GaussianMatrix=new float*[size];
    for (int i = 0; i < size; i++)
    {
        GaussianMatrix[i]=new float[size];
        for (int j = 0; j < size; j++)
        {
            GaussianMatrix[i][j] = coeff * (float)exp(-(pow(fabs(size/2-i),2)+pow(fabs(size/2-j),2))/(2*sigma*sigma));
            printf("%f ",GaussianMatrix[i][j]);
        }
        printf("\n");
    }
    return GaussianMatrix;
}

int ** Cartoon::Histogram(int *** arrImage, int height, int width) {
    int ** HistogramArray=new int * [COUNT_OF_COLORS];
    
    for (int i=0; i<COUNT_OF_COLORS; i++) {
        HistogramArray[i]=new int[COUNT_OF_CHANNELS];
        memset(HistogramArray, 0, COUNT_OF_COLORS * sizeof(int));
    }
    
    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
            for (int k=0; k<COUNT_OF_CHANNELS; k++)
                HistogramArray[arrImage[i][j][k]][k]++;
    
    return HistogramArray;
}

void Cartoon::cartoonFilter(int *** arrImage, int height, int width) {
    int maskRadius=40;
    double threshold=0.9;
    float ramp=0.2;
    
    float ** GaussianMatrix=GaussianFunction(0.9,maskRadius);
    
    int center = maskRadius / 2 + 1;
    
    // Проход по строкам
    
    for (int i=0; i<width; i++) {
        for (int x=i-maskRadius-1; x<i+maskRadius; x++) {
            if (x>=0 && x<width) {
                for (int j=0; j<height; j++) {
                    for (int y=j-maskRadius-1; y<j+maskRadius; y++) {
                        if (y>=0 && y<height) {
                            float diffR=GaussianMatrix[x-i+maskRadius+1][y-j+maskRadius+1];//*arrImage[x][y][2];
                            if (diffR<threshold) {
                                arrImage[x][y][2]*=((ramp - fmin(ramp,(threshold - diffR)))/ramp);
                                //printf("%f ",((ramp - fmin(ramp,(threshold - diffR)))/ramp));
                            }
                            diffR=arrImage[x][y][2];
                            float diffG=GaussianMatrix[x-i+maskRadius+1][y-j+maskRadius+1];//*arrImage[x][y][1];
                            if (diffG<threshold)
                                arrImage[x][y][1]*=((ramp - fmin(ramp,(threshold - diffG)))/ramp);
                            float diffB=GaussianMatrix[x-i+maskRadius+1][y-j+maskRadius+1];//*arrImage[x][y][0];
                            if (diffB<threshold)
                                arrImage[x][y][0]*=((ramp - fmin(ramp,(threshold - diffB)))/ramp);
                            
                        }
                    }
                }
            }
        }
    }
    
}



void Cartoon::cartoonFilterWithAverageValues(int *** arrImage, int height, int width) {
    int maskRadius=40;
    double threshold=0.9;
    float ramp=0.15;
    
    int center = maskRadius / 2 + 1;
    
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++){
    
            int i = 0;
            double sumR = 0, sumB = 0, sumG = 0;

            for(int iX = x; i < maskRadius; ++i, ++iX){
                if  (iX==width) break;
                int j = 0;
                for(int iY = y; j < maskRadius; ++j, ++iY){
                    if (iY==height) break;
                    sumR += arrImage[iX][iY][2];
                    sumB += arrImage[iX][iY][1];
                    sumG += arrImage[iX][iY][0];
                }
            }
            
            
            sumR /= maskRadius*maskRadius;
                sumB /= maskRadius*maskRadius;
                sumG /= maskRadius*maskRadius;
                
                double red = arrImage[x][y][2],
                blue = arrImage[x][y][1],
                green = arrImage[x][y][0];

                double koeffR = red / sumR,
                koeffB = blue / sumB,
                koeffG = green / sumG;
                
                if(koeffR < threshold)
                    red *= ((ramp - fmin(ramp,(threshold - koeffR)))/ramp);

                if(koeffB < threshold)
                    blue *= ((ramp - fmin(ramp,(threshold - koeffB)))/ramp);

                if(koeffG < threshold)
                    green *= ((ramp - fmin(ramp,(threshold - koeffG)))/ramp);
                
                arrImage[x][y][2]=(int)red;
                arrImage[x][y][1]=(int)blue;
                arrImage[x][y][0]=(int)green;
                
                //if (iX>100) break;
            
            
            
        }
    }


    
    
}
