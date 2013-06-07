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
    IplImage * image = 0;
    IplImage * src=0;
    
    /*FILE *f;
    //for (int k=0; k<10; k++) {
        
        
        char name[13];
        sprintf(name,"img.jpg");
        f=fopen("/users/madmoron/img.jpg", "r");
    //f=fopen(name, "w");
    
        
        
        fclose(f);*/
    //}
    //std::ofstream ;//<"test.txt">; //создать
    
    //ofs.close();
    
    const char * filename = argc == 2 ? argv[1] : "/users/madmoron/Desktop/img.jpg";
    // получаем картинку
    image = cvLoadImage(filename,1);
    // клонируем картинку
    src = cvCloneImage(image);
    
    printf("[i] image: %s\n", filename);
    assert( src != 0 );
    
    // окно для отображения картинки
    //cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
    
    // показываем картинку
    //cvShowImage("original",image);
    
    // выводим в консоль информацию о картинке
    printf( "[i] channels:    %d\n",        image->nChannels );
    printf( "[i] pixel depth: %d bits\n",   image->depth );
    printf( "[i] width:       %d pixels\n", image->width );
    printf( "[i] height:      %d pixels\n", image->height );
    printf( "[i] image size:  %d bytes\n",  image->imageSize );
    printf( "[i] width step:  %d bytes\n",  image->widthStep );
    
    // ждём нажатия клавиши
    //cvWaitKey(0);
    
    
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
    printf("%d %d %d \n%d %d %d",arrImage[0][0][2],arrImage[0][0][1],arrImage[0][0][0],arrImage[1][1][2],arrImage[1][1][1],arrImage[1][1][0]);
    
    //printf("%d",arrImage[0][0][0]);
    
    //int *** newArrImage
    
    //cartoonFilter(arrImage, image->width, image->height, arrImage);
    
    //Cartoon::cartoonFilter(arrImage, <#int height#>, <#int width#>, <#int ***newArrImage#>)
    
    Cartoon::cartoonFilter(arrImage, image->width, image->height, arrImage);
    
    // собрать новое изображение
    
    for( int y=0; y<src->height; y++ ) {
        char * ptr = (char *) (src->imageData + y * src->widthStep);
        for (int x=0; x<src->width; x++) {
            ptr[3*x]=(char)(arrImage[y][x][0]);     // blue
            if (ptr[3*x]>127) ptr[3*x]-=256;
            ptr[3*x+1]=(char)(arrImage[y][x][1]);   // green
            if (ptr[3*x+1]>127) ptr[3*x+1]-=256;
            ptr[3*x+2]=(char)(arrImage[y][x][2]);   // red
            if (ptr[3*x+2]>127) ptr[3*x+2]-=256;
            
        }
    }
    
    
    cvSaveImage("/users/madmoron/Desktop/img2.jpg", src, 0);
    
   
    
    
    hello();
    
    
    // освобождаем ресурсы
    cvReleaseImage(& image);
    cvReleaseImage(& src);
    // удаляем окно
    //cvDestroyWindow("original");
    return 0;

}

