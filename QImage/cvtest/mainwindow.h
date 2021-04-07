/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author: Deng jinhui<dengjinhui@uniontech.com>
*
* Maintainer: Deng jinhui <dengjinhui@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <QDebug>

using namespace cv;

namespace Ui {
class MainWindow;
}

void elbp(Mat &src, Mat &dst, int radius, int neighbors);
void elbp1(Mat &src, Mat &dst);



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    bool checkMat();
    void verfilter(int sigema);
    void horfilter(int sigema);
    void gauss(int sigema);
    void sobel();
    void lbp();
    void classicalfilter();
    void disorderColor();
    void simpleColor(int color);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::MainWindow *ui;
    QImage m_image;
    Mat m_mat;
};


class HistogramND
{

private:
    Mat image;//源图像
    int hisSize[1], hisWidth, hisHeight;//直方图的大小,宽度和高度
    float range[2];//直方图取值范围
    const float *ranges;
    Mat channelsRGB[3];//分离的BGR通道
    MatND outputRGB[3];//输出直方图分量
public:
    HistogramND()
    {
        hisSize[0] = 256;
        hisWidth = 400;
        hisHeight = 400;
        range[0] = 0.0;
        range[1] = 255.0;
        ranges = &range[0];
    }

    //导入图片
    bool importImage(QString path)
    {
        image = imread(path.toStdString());
        if (!image.data)
            return false;
        return true;
    }

    void setImage(Mat m)
    {
        image = m;
    }
    //分离通道
    void splitChannels()
    {
        split(image, channelsRGB);
    }

    //计算直方图
    void getHistogram()
    {
        calcHist(&channelsRGB[0], 1, nullptr, Mat(), outputRGB[0], 1, hisSize, &ranges);
        calcHist(&channelsRGB[1], 1, nullptr, Mat(), outputRGB[1], 1, hisSize, &ranges);
        calcHist(&channelsRGB[2], 1, nullptr, Mat(), outputRGB[2], 1, hisSize, &ranges);

        //输出各个bin的值
        for (int i = 0; i < hisSize[0]; ++i) {
            qDebug() << i << "   B:" << outputRGB[0].at<float>(i);
            qDebug() << "   G:" << outputRGB[1].at<float>(i);
            qDebug() << "   R:" << outputRGB[2].at<float>(i);
        }
    }

    //显示直方图
    void displayHisttogram()
    {
        Mat rgbHist[3];
        for (int i = 0; i < 3; i++) {
            rgbHist[i] = Mat(hisWidth, hisHeight, CV_8UC3, Scalar::all(0));
        }
        normalize(outputRGB[0], outputRGB[0], 0, hisWidth - 20, NORM_MINMAX);
        normalize(outputRGB[1], outputRGB[1], 0, hisWidth - 20, NORM_MINMAX);
        normalize(outputRGB[2], outputRGB[2], 0, hisWidth - 20, NORM_MINMAX);
        for (int i = 0; i < hisSize[0]; i++) {
            int val = saturate_cast<int>(outputRGB[0].at<float>(i));
            rectangle(rgbHist[0], Point(i * 2 + 10, rgbHist[0].rows), Point((i + 1) * 2 + 10, rgbHist[0].rows - val), Scalar(0, 0, 255), 1, 8);
            val = saturate_cast<int>(outputRGB[1].at<float>(i));
            rectangle(rgbHist[1], Point(i * 2 + 10, rgbHist[1].rows), Point((i + 1) * 2 + 10, rgbHist[1].rows - val), Scalar(0, 255, 0), 1, 8);
            val = saturate_cast<int>(outputRGB[2].at<float>(i));
            rectangle(rgbHist[2], Point(i * 2 + 10, rgbHist[2].rows), Point((i + 1) * 2 + 10, rgbHist[2].rows - val), Scalar(255, 0, 0), 1, 8);
        }

        cv::imshow("R", rgbHist[0]);
        imshow("G", rgbHist[1]);
        imshow("B", rgbHist[2]);
        imshow("image", image);
    }
};



#endif // MAINWINDOW_H
