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
    0.7,   /* ramp         */
    /* Для фильтра с вычислением средних значений лучше использовать значение ramp меньше, например, 0.15 */
    0.45,   /* sigma        */
    3       /* blurRadius   */
};

float * Cartoon::GaussianFunction(float sigma, int blurRadius)
{
    int size=2*blurRadius+1;
    float coeff = 1.0f/(2.0f*(float)M_PI*sigma*sigma);
    float * GaussianMatrix=new float[size];
    for (int i = 0; i < size; i++)
        GaussianMatrix[i] = coeff * (float)exp(-(pow(fabs(size/2-i),2)+pow(fabs(size/2-blurRadius),2))/(2*sigma*sigma));
    return GaussianMatrix;
}

/*  Гистограмма — это график распределения полутонов изображения, в котором по горизонтальной оси представлена яркость,       а по вертикали — относительное число пикселов с данным значением яркости.
    В данном случае гистограмма - массивы каждой составляющей RGB со значениями яркости 0..255 */
int ** Cartoon::Histogram(int *** arrImage, int height, int width)
{
    int ** HistogramArray=new int * [COUNT_OF_COLORS];
    
    for (int i=0; i<COUNT_OF_COLORS; i++) {
        HistogramArray[i]=new int[COUNT_OF_CHANNELS];
        memset(HistogramArray[i], 0, COUNT_OF_CHANNELS * sizeof(int));
    }
    
    for (int i=0; i<width; i++)
        for (int j=0; j<height; j++)
            for (int k=0; k<COUNT_OF_CHANNELS; k++)
                HistogramArray[arrImage[i][j][k]][k]++; 
    
    return HistogramArray;
}

/*  Cartoon-алгоритм с использованием размытия по Гауссу
    Сначала производится размытие изображения при помощи фильтра Гаусса. Фильтр Гаусса проходит сначала по горизонтали, а потом по вертикали. 
    Далее находится гистограмма изображения, при помощи которой вычисляется ramp. Ramp - сумма относительной разности интенсивности до полного черного.
    Затем определяется относительная интенсивность пикселя diff = интенсивность пикселя / среднее значение в радиусе maskRadius.
    Если относительная разница меньше порогового значения diff < threshold, то множитель интенсивности mult = (ramp - MIN (ramp, (threshold - diff))).
    Интенсивность цвета текущего пикселя умножается на mult.
    Таким образом, алгоритм ищет самые темные области на изображении и затемняет их больше.
*/
void Cartoon::cartoonFilterWithGaussianBlur(int *** arrImage, int height, int width, int maskRadius, float threshold, float ramp, float sigma, int blurRadius) {
    
    
    float * GaussianMatrix=GaussianFunction(sigma,blurRadius);
    
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
                        pixel[m]+=(float)arrImage[l][j][m]*GaussianMatrix[k+blurRadius];
                        sum[m]+=arrImage[l][j][m];
                    }
                }
                else {// Область размытия выходит за рамки изображения
                    for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                        pixel[m]+=(float)arrImage[i][j][m]*GaussianMatrix[k+blurRadius];
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
                        pixel[m]+=(float)arrImage[i][l][m]*GaussianMatrix[k+blurRadius];
                        sum[m]+=arrImage[i][l][m];
                    }
                }
                else {// Область размытия выходит за рамки изображения
                    for (int m=0; m<COUNT_OF_CHANNELS; m++) {
                        pixel[m]+=(float)arrImage[i][j][m]*GaussianMatrix[k+blurRadius];
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
    
    // Нахождение гистограммы изображения
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
            
            // Нахождение средних значений
            for(int iX = x; i < maskRadius; ++i, ++iX){
                if  (iX==width) break;
                int j = 0;
                for(int iY = y; j < maskRadius; ++j, ++iY){
                    if (iY==height) break;
                    for (int k=0; k<COUNT_OF_CHANNELS; k++) 
                        avr[k] += arrImage[iX][iY][k];
                }
            }
            
            // Поиск множителя интенсивности цвета
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
            
            delete avr;
        }
    }
    
    delete hist;
    delete GaussianMatrix;
    
    
}

void Cartoon::cartoonFilterWithGaussianBlur(int *** arrImage, int height, int width) {
    cartoonFilterWithGaussianBlur(arrImage, height, width, parameters.maskRadius, parameters.threshold, parameters.ramp, parameters.sigma, parameters.blurRadius);
}

/*  Cartoon-алгоритм с использованием средних значений
    Упрощенный алгоритм, которые находит средние значения в радиусе maskRadius.
    Ramp - сумма относительной разности интенсивности до полного черного, в данном случае задается пользователем.
    Сначала определяется относительная интенсивность пикселя diff = интенсивность пикселя / среднее значение в радиусе maskRadius.
    Если относительная разница меньше порогового значения diff < threshold, то множитель интенсивности mult = (ramp - MIN (ramp, (threshold - diff))).
    Интенсивность цвета текущего пикселя умножается на mult.
    Таким образом, алгоритм ищет самые темные области на изображении и затемняет их больше.
*/
void Cartoon::cartoonFilterWithAverageValues(int *** arrImage, int height, int width,  int maskRadius, float threshold, float ramp) {
    
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++){
    
            int i = 0;
            
            float * avr=new float[COUNT_OF_CHANNELS];
            memset(avr,0.0,COUNT_OF_CHANNELS*sizeof(float));
            
            // Нахождение средних значений
            for(int iX = x; i < maskRadius; ++i, ++iX){
                if  (iX==width) break;
                int j = 0;
                for(int iY = y; j < maskRadius; ++j, ++iY){
                    if (iY==height) break;
                    for (int k=0; k<COUNT_OF_CHANNELS; k++)
                        avr[k] += arrImage[iX][iY][k];
                }
            }
            
            // Поиск множителя интенсивности цвета
            for (int k=0; k<COUNT_OF_CHANNELS; k++) {
                avr[k]/=maskRadius*maskRadius;
                float diff=arrImage[x][y][k]/avr[k];
                float mult=1.0;
                if (diff<threshold) {
                    if (ramp==0.0) mult=0.0;
                    else mult=(ramp - fmin(ramp,(threshold - diff)))/ramp;
                }
                arrImage[x][y][k]=(int)(arrImage[x][y][k]*mult);
            }
            
            delete avr;
        }
    }
}

void Cartoon::cartoonFilterWithAverageValues(int *** arrImage, int height, int width) {
    cartoonFilterWithAverageValues(arrImage, height, width, parameters.maskRadius, parameters.threshold, parameters.ramp);
}
