//
//  cartoon.h
//  filter
//
//  Created by Дарья Кабанова on 07.06.13.
//  Copyright (c) 2013 Дарья Кабанова. All rights reserved.
//

#ifndef __filter__cartoon__
#define __filter__cartoon__

#include <iostream>

typedef struct
{
    int     maskRadius; /* радиус окрестности пикселя для сравнения интенсивности */
    float   threshold;  /* порог относительной разности интенсивности */
    float   ramp;       /* сумма относительной разности интенсивности до полного черного */
    float   sigma;      /* среднеквадратическое отклонение */
    int     blurRadius; /* радиус размытия */
} CartoonParameters;


class Cartoon {
public:
    static CartoonParameters parameters; /* структура с параметрами фильтра */
    
    /* Cartoon-алгоритм с использованием размытия по Гауссу
     * arrImage массив с исходными цветами пикселей
     * height   количество пикселей в высоту изображения
     * width    количество пикселей в длину изображения
     */
    static void cartoonFilterWithGaussianBlur(int *** arrImage, int height, int width);
    
    
    static void cartoonFilterWithAverageValues(int *** arrImage, int height, int width);
    
protected:
    
    /* Нахождение значений функции Гаусса */
    static float * GaussianFunction();
    
    /* Нахождение гистограммы изображения
     * arrImage массив с исходными цветами пикселей
     * height   количество пикселей в высоту изображения
     * width    количество пикселей в длину изображения
     */
    static int ** Histogram(int *** arrImage, int height, int width);
    
    static float computeRamp(int ** arrRow);
};


#endif /* defined(__filter__cartoon__) */
