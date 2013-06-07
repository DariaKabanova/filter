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

//#include <opencv/cvaux.hpp>

//using namespace filter;

int main(int argc, const char * argv[])
{
    IplImage * image = 0;       // Исходное изображение, представленное структурой из OpenCV
    IplImage * imageCopy = 0;   // Копия исходного изображения
    
    const char * filename = argc == 2 ? argv[1] : "/users/madmoron/Desktop/img.jpg";
    
    // получаем картинку
    image = cvLoadImage(filename,1);
    
    if (!image) return -1;
    
    // клонируем картинку
    imageCopy = cvCloneImage(image);
    
    //assert( imageCopy != 0 );
    
    /* отображание окна с изображением
    // окно для отображения картинки
    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
    
    // показываем картинку
    cvShowImage("original",image);
    
    // ждём нажатия клавиши
    cvWaitKey(0);
    */
    
    // пробегаемся по всем пикселям изображения
    int *** arrImage=new int **[image->height];
    for( int y=0; y<image->height; y++ ) {
        char * ptr = (char *) (image->imageData + y * image->widthStep);
        arrImage[y]=new int * [image->width];
        for( int x=0; x<image->width; x++ ) {
            arrImage[y][x]=new int[3];
            // 3 канала
            arrImage[y][x][0]=(int)ptr[3*x];
            if (arrImage[y][x][0]<0) arrImage[y][x][0]+=256;
            //printf("%d",(int)ptr[3*x]);
            // B - синий
            arrImage[y][x][1]=(int)ptr[3*x+1];
            if (arrImage[y][x][1]<0) arrImage[y][x][1]+=256;
            // G - зелёный
            arrImage[y][x][2] = (int)ptr[3*x+2];
            if (arrImage[y][x][2]<0) arrImage[y][x][2]+=256;
            // R - красный
        }
    }
    //printf("%d %d %d \n%d %d %d",arrImage[0][0][2],arrImage[0][0][1],arrImage[0][0][0],arrImage[1][1][2],arrImage[1][1][1],arrImage[1][1][0]);
    
        
    Cartoon::cartoonFilter(arrImage, image->width, image->height);
    
    // собрать новое изображение    
    for( int y=0; y<imageCopy->height; y++ ) {
        char * ptr = (char *) (imageCopy->imageData + y * imageCopy->widthStep);
        for (int x=0; x<imageCopy->width; x++) {
            ptr[3*x]=(char)(arrImage[y][x][0]);     // blue
            if (ptr[3*x]>127) ptr[3*x]-=256;
            ptr[3*x+1]=(char)(arrImage[y][x][1]);   // green
            if (ptr[3*x+1]>127) ptr[3*x+1]-=256;
            ptr[3*x+2]=(char)(arrImage[y][x][2]);   // red
            if (ptr[3*x+2]>127) ptr[3*x+2]-=256;
            
        }
    }
    
    
    cvSaveImage("/users/madmoron/Desktop/img2.jpg", imageCopy, 0);
    
   
    
    
    
    
    
    // освобождение ресурсов
    cvReleaseImage(& image);
    cvReleaseImage(& imageCopy);
    
    // удаляем окно
    //cvDestroyWindow("original");
    
    return 0;

}

