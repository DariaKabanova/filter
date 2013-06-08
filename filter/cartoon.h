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

void hello(void);


class Cartoon {
    //Cartoon();
    public:
        static void cartoonFilter(int *** arrImage, int height, int width);
        static void cartoonFilterWithAverageValues(int *** arrImage, int height, int width);
    protected:
        static float ** GaussianFunction(float sigma,int radius);
        static int ** Histogram(int *** arrImage, int height, int width);
    
};

/*
 @param 
 */
//void cartoonFilter(int *** arrImage, int height, int width, int *** newArrImage);

#endif /* defined(__filter__cartoon__) */
