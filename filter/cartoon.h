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
} CartoonParameters;


class Cartoon {
public:
    static CartoonParameters parameters; /* параметры фильтра */
    static void cartoonFilter(int *** arrImage, int height, int width);
    static void cartoonFilterWithAverageValues(int *** arrImage, int height, int width);
    
protected:
    static float ** GaussianFunction(float sigma,int radius);
    static int ** Histogram(int *** arrImage, int height, int width);    
};


#endif /* defined(__filter__cartoon__) */
