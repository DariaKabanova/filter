//
//  main.cpp
//  filter
//
//  Created by Дарья Кабанова on 06.06.13.
//  Copyright (c) 2013 Дарья Кабанова. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cartoon.h"

#define COUNT_OF_CHANNELS   3 // Количество каналов цветов

// Работа с изображением при помощи OpenCV

// Получить массив с цветами всех пикселей изображения BGR

int *** getImageArray (IplImage * image)
{
    int *** arrImage=new int **[image->height]; // Массив с цветами, который получается на выходе
    
    for( int y=0; y<image->height; y++ ) {
        char * ptr = (char *) (image->imageData + y * image->widthStep);
        arrImage[y]=new int * [image->width];
        for( int x=0; x<image->width; x++ ) {
            arrImage[y][x]=new int[COUNT_OF_CHANNELS];
            
            for (int i=0; i<COUNT_OF_CHANNELS; i++) { // Получение цветов из всех каналов
                arrImage[y][x][i]=(int)ptr[COUNT_OF_CHANNELS*x+i];
                if (arrImage[y][x][i]<0) arrImage[y][x][i]+=256;
            }
        }
    }
    
    return arrImage;
}

// Собрать новое изображение из обработанного массива с цветами всех пикселей изображения BGR

IplImage * collectImageFromArray (IplImage * image, int *** arrImage)
{
    IplImage * newImage = 0;
    // клонирование изображения
    newImage = cvCloneImage(image);
    
    for( int y=0; y<newImage->height; y++ ) {
        char * ptr = (char *) (newImage->imageData + y * newImage->widthStep);
        for (int x=0; x<newImage->width; x++) {
            for (int i=0; i<COUNT_OF_CHANNELS; i++) {
                ptr[COUNT_OF_CHANNELS*x+i]=(char)(arrImage[y][x][i]);
                if (ptr[COUNT_OF_CHANNELS*x+i]>127) ptr[COUNT_OF_CHANNELS*x+i]-=256;
            }
        }
    }
    
    return newImage;
}


int main(int argc, const char * argv[])
{
    IplImage * image = 0;  // Исходное изображение, представленное структурой из OpenCV
    
    const char * filename = argc == 2 ? argv[1] : "/users/madmoron/Desktop/img.jpg";
    
    // получаем картинку
    image = cvLoadImage(filename,1);
    
    if (!image) return -1; // Если изображение не найдено, то возвращается код ошибки
    
    /* отображание окна с изображением
     // окно для отображения картинки
     cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
     
     // показываем картинку
     cvShowImage("original",image);
     
     // ждём нажатия клавиши
     cvWaitKey(0);
     */
    
    // получение массива с цветами всех пикселей
    int *** arrImage = getImageArray(image);
    
    // применение фильтра
    Cartoon::parameters={4,0.9,0.2};
    Cartoon::cartoonFilterWithAverageValues(arrImage, image->width, image->height);
    
    // сборка нового изображения
    IplImage * newImage = collectImageFromArray(image,arrImage);
    
    // сохранение изображения
    cvSaveImage("/users/madmoron/Desktop/img2.jpg", newImage, 0);
    
    // освобождение ресурсов
    cvReleaseImage(& image);
    cvReleaseImage(& newImage);
    
    // удаляем окно
    //cvDestroyWindow("original");
    
    return 0;
    
}

