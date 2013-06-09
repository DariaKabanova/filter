//
//  cartoon.cpp
//  filter
//
//  Created by Дарья Кабанова on 07.06.13.
//  Copyright (c) 2013 Дарья Кабанова. All rights reserved.
//

#include "cartoon.h"
#include <math.h>
#define COUNT_OF_COLORS     256
#define COUNT_OF_CHANNELS   3

CartoonParameters Cartoon::parameters =
{
    5,      /* maskRadius   */
    1.0,    /* threshold    */
    0.9    /* ramp         */
};

float ** Cartoon::GaussianFunction(float sigma,int radius)
{
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

int ** Cartoon::Histogram(int *** arrImage, int height, int width)
{
    int ** HistogramArray=new int * [COUNT_OF_COLORS];
    
    for (int i=0; i<COUNT_OF_COLORS; i++) {
        HistogramArray[i]=new int[COUNT_OF_CHANNELS];
        memset(HistogramArray[i], 0, COUNT_OF_CHANNELS * sizeof(int));
    }
    
    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
            for (int k=0; k<COUNT_OF_CHANNELS; k++) {
                HistogramArray[arrImage[i][j][k]][k]++;
            }
    
    
    
    return HistogramArray;
}

void Cartoon::cartoonFilter(int *** arrImage, int height, int width) {
    int maskRadius=parameters.maskRadius;
    float threshold=parameters.threshold;
    float ramp=parameters.ramp;
    int blurRadius=2;
    
    float ** GaussianMatrix=GaussianFunction(0.43,blurRadius);
    
    int center = maskRadius / 2 + 1;

    
    // Горизонтальное размытие
    for (int j=0; j<height; j++) {
        int ** tempRow=new int * [width];// временная строка
        for (int i=0; i<width; i++) {
            // Вычисление суммы использованных коэффициентов для нормализации
            float sum[COUNT_OF_CHANNELS];
            int pixel[COUNT_OF_CHANNELS];
            memset(pixel,0,COUNT_OF_CHANNELS * sizeof(int));
            memset(sum,0,COUNT_OF_CHANNELS * sizeof(int));
            
            // Проход вокруг пикселя [i][j]
            for (int k=-blurRadius; k<=blurRadius; k++) {
                // l - индекс другого пикселя который вносит вклад
                int l=i+k;
                if (l>=0 && l<width) {
                    for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                        pixel[m]+=(float)arrImage[l][j][m]*GaussianMatrix[blurRadius][k+blurRadius];
                        sum[m]+=arrImage[l][j][m];
                    }
                }
                else {// Область размытия выходит за рамки изображения
                    for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                        pixel[m]+=(float)arrImage[i][j][m]*GaussianMatrix[blurRadius][k+blurRadius];
                        sum[m]+=arrImage[i][j][m];
                    }
                }
            }
            // Нормализация: сумма использованных коэффициентов должна быть равна 1
            for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                
                pixel[m]=(float)(pixel[m]*pixel[m]*(2*blurRadius+1))/sum[m];
                if ((float)pixel[m]*(2*blurRadius+1)/sum[m]>=0.5)
                    pixel[m]++;
            }
            
            tempRow[i]=new int[COUNT_OF_CHANNELS];
            for (int m=0; m<COUNT_OF_CHANNELS; m++)
                tempRow[i][m]=pixel[m];
        }
        for (int i=0; i<width; i++)
            for (int m=0; m<COUNT_OF_CHANNELS; m++)
                if (tempRow[i][m]<0) arrImage[i][j][m]=0;
                else if (tempRow[i][m]>255) arrImage[i][j][m]=255;
                else arrImage[i][j][m]=tempRow[i][m];
        delete tempRow;
    }
    
    // Вертикальное размытие
    for (int i=0; i<width; i++) {
        int ** tempColumn=new int * [height];// временная строка
        for (int j=0; j<height; j++) {
            // Вычисление суммы использованных коэффициентов для нормализации
            float sum[COUNT_OF_CHANNELS];
            int pixel[COUNT_OF_CHANNELS];
            memset(pixel,0,COUNT_OF_CHANNELS * sizeof(int));
            memset(sum,0,COUNT_OF_CHANNELS * sizeof(int));
            
            // Проход вокруг пикселя [i][j]
            for (int k=-blurRadius; k<=blurRadius; k++) {
                // l - индекс другого пикселя который вносит вклад
                int l=j+k;
                if (l>=0 && l<height) {
                    for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                        pixel[m]+=(float)arrImage[i][l][m]*GaussianMatrix[blurRadius][k+blurRadius];
                        sum[m]+=arrImage[i][l][m];
                    }
                }
                else {// Область размытия выходит за рамки изображения
                    for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                        pixel[m]+=(float)arrImage[i][j][m]*GaussianMatrix[blurRadius][k+blurRadius];
                        sum[m]+=arrImage[i][j][m];
                    }
                }
            }
            // Нормализация: сумма использованных коэффициентов должна быть равна 1
            for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                
                pixel[m]=(float)(pixel[m]*pixel[m]*(2*blurRadius+1))/sum[m];
                if ((float)pixel[m]*(2*blurRadius+1)/sum[m]>=0.5)
                    pixel[m]++;
            }
            
            tempColumn[j]=new int[COUNT_OF_CHANNELS];
            for (int m=0; m<COUNT_OF_CHANNELS; m++)
                tempColumn[j][m]=pixel[m];
        }
        for (int j=0; j<height; j++)
            for (int m=0; m<COUNT_OF_CHANNELS; m++)
                if (tempColumn[j][m]<0) arrImage[i][j][m]=0;
                else if (tempColumn[j][m]>255) arrImage[i][j][m]=255;
                else arrImage[i][j][m]=tempColumn[j][m];
        delete tempColumn;
    }
    
    
    
    int ** hist=Histogram(arrImage, height, width);
    
    float ramps[COUNT_OF_CHANNELS];
    memset(ramps, 0.0, COUNT_OF_CHANNELS * sizeof(float));
    
    for (int k=0; k<COUNT_OF_CHANNELS; k++) {
        int count=0;
        for (int i=0; i<COUNT_OF_COLORS; i++) {
            count+=hist[i][k];
        }
        int sum=0;
        for (int i=0; i<COUNT_OF_COLORS; i++) {
            sum+=hist[i][k];
            if ((float)sum/(float)count>ramp) {
                ramps[k]=1.0-(float)i/(float)COUNT_OF_COLORS;
                break;
            }
        }
    }
    
    for(int x = 0; x < width; x++) {
        
        
        
        for(int y = 0; y < height; y++){
            
            int i = 0;
            
            float * avr=new float[COUNT_OF_CHANNELS];
            memset(avr,0.0,COUNT_OF_CHANNELS*sizeof(float));
            
            for(int iX = x; i < maskRadius; ++i, ++iX){
                if  (iX==width) break;
                int j = 0;
                for(int iY = y; j < maskRadius; ++j, ++iY){
                    if (iY==height) break;
                    for (int k=0; k<COUNT_OF_CHANNELS; k++) 
                        avr[k] += arrImage[iX][iY][k];
                }
            }
            
            for (int k=0; k<COUNT_OF_CHANNELS; k++) {
                avr[k]/=maskRadius*maskRadius;
                float diff=arrImage[x][y][k]/avr[k];
                float mult=1.0;
                if (diff<threshold) {
                    if (ramps[k]==0.0) mult=0.0;
                    else mult=(ramps[k] - fmin(ramps[k],(threshold - diff)))/ramps[k];
                }
                arrImage[x][y][k]=(int)(arrImage[x][y][k]*mult);
            }
        }
    }
    
    delete GaussianMatrix;
    
}

float Cartoon::computeRamp(int ** arrRow) {
    float diff_black=parameters.ramp;
    
    
    float ramp=diff_black;
    return ramp;
}


void Cartoon::cartoonFilterWithAverageValues(int *** arrImage, int height, int width) {
    int maskRadius=parameters.maskRadius;
    float threshold=parameters.threshold;
    float ramp=parameters.ramp;
    
    //Добавить прохождение до конца
    
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
