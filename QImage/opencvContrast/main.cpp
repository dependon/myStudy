#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat g_srcImage,g_dstImage;
int g_nContrast,g_nBright;

void On_ContrastAndBright(int,void*)
{
    namedWindow("原始图",1);
    for (int a = 0;a<g_srcImage.rows;a++) {
        for (int b=0;b < g_srcImage.cols; b++) {
            for (int c=0;c<3;c++) {
                g_dstImage.at<Vec3b>(a,b)[c] = saturate_cast<uchar>((g_nContrast*0.01)*(g_srcImage.at<Vec3b>(a,b)[c])+g_nBright);
            }
        }
    }
    imshow("原始图",g_srcImage);
    imshow("效果图",g_dstImage);
}

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//    Mat srcImage = imread("/home/shuwenzhi/Pictures/桌面测试图片/1.bmp");
//    Mat element =getStructuringElement(MORPH_RECT,Size(15,15));
//    Mat dstImage;
//    erode(srcImage,dstImage,element);
//    imshow("原始图",dstImage);
       g_srcImage=imread("/data/home/lmh/1030/1/Camera/IMG_20190707_202427.jpg",IMREAD_REDUCED_COLOR_4);
       g_dstImage=Mat::zeros(g_srcImage.size(),g_srcImage.type());
       g_nContrast = 80;
       g_nBright = 80;
       namedWindow("效果图",1);
       createTrackbar("对比度","效果图",&g_nContrast,300,On_ContrastAndBright);
       createTrackbar("亮 度","效果图",&g_nBright,300,On_ContrastAndBright);
       On_ContrastAndBright(g_nContrast,0);
       On_ContrastAndBright(g_nBright,0);
       while (char(waitKey(1)) != 'q') {

       }
       //cvtColor(srcImage,dstImage,COLOR_BGR2Lab);
//       imshow("效果图",srcImage);
//    waitKey(0);
    return 0;
}
