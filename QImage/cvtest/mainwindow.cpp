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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImageReader>
#include <QDebug>

void elbp(Mat &src, Mat &dst, int radius, int neighbors)
{
    for (int n = 0; n < neighbors; n++) {
        // 采样点的计算
        float x = static_cast<float>(-radius * sin(2.0 * CV_PI * n / static_cast<double>(neighbors)));
        float y = static_cast<float>(radius * cos(2.0 * CV_PI * n / static_cast<double>(neighbors)));
        // 上取整和下取整的值
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        // 小数部分
        float ty = y - fy;
        float tx = x - fx;
        // 设置插值权重
        float w1 = (1 - tx) * (1 - ty);
        float w2 = tx  * (1 - ty);
        float w3 = (1 - tx) * ty;
        float w4 = tx * ty;
        // 循环处理图像数据
        for (int i = radius; i < src.rows - radius; i++) {
            for (int j = radius; j < src.cols - radius; j++) {
                // 计算插值
                float t = static_cast<float>(w1 * src.at<uchar>(i + fy, j + fx) + w2 * src.at<uchar>(i + fy, j + cx) + w3 * src.at<uchar>(i + cy, j + fx) + w4 * src.at<uchar>(i + cy, j + cx));
                // 进行编码
                dst.at<uchar>(i - radius, j - radius) += ((t > src.at<uchar>(i, j)) || (std::abs(t - src.at<uchar>(i, j)) < std::numeric_limits<float>::epsilon())) << n;
            }
        }
    }
}

void elbp1(Mat &src, Mat &dst)
{
    // 循环处理图像数据
    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            uchar tt = 0;
            int tt1 = 0;
            uchar u = src.at<uchar>(i, j);
            if (src.at<uchar>(i - 1, j - 1) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i - 1, j) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i - 1, j + 1) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i, j + 1) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i + 1, j + 1) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i + 1, j) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i + 1, j - 1) > u) {
                tt += 1 << tt1;
            }
            tt1++;
            if (src.at<uchar>(i - 1, j) > u) {
                tt += 1 << tt1;
            }
            tt1++;

            dst.at<uchar>(i - 1, j - 1) = tt;
        }
    }
}

template <typename _tp>
void getCircularLBPFeatureOptimization(InputArray _src, OutputArray _dst, int radius, int neighbors)
{
    Mat src = _src.getMat();
    //LBP特征图像的行数和列数的计算要准确
    _dst.create(src.rows - 2 * radius, src.cols - 2 * radius, CV_8UC1);
    Mat dst = _dst.getMat();
    dst.setTo(0);
    for (int k = 0; k < neighbors; k++) {
        //计算采样点对于中心点坐标的偏移量rx，ry
        float rx = static_cast<float>(radius * cos(2.0 * CV_PI * k / neighbors));
        float ry = -static_cast<float>(radius * sin(2.0 * CV_PI * k / neighbors));
        //为双线性插值做准备
        //对采样点偏移量分别进行上下取整
        int x1 = static_cast<int>(floor(rx));
        int x2 = static_cast<int>(ceil(rx));
        int y1 = static_cast<int>(floor(ry));
        int y2 = static_cast<int>(ceil(ry));
        //将坐标偏移量映射到0-1之间
        float tx = rx - x1;
        float ty = ry - y1;
        //根据0-1之间的x，y的权重计算公式计算权重，权重与坐标具体位置无关，与坐标间的差值有关
        float w1 = (1 - tx) * (1 - ty);
        float w2 = tx * (1 - ty);
        float w3 = (1 - tx) * ty;
        float w4 = tx * ty;
        //循环处理每个像素
        for (int i = radius; i < src.rows - radius; i++) {
            for (int j = radius; j < src.cols - radius; j++) {
                //获得中心像素点的灰度值
                _tp center = src.at<_tp>(i, j);
                //根据双线性插值公式计算第k个采样点的灰度值
                float neighbor = src.at<_tp>(i + x1, j + y1) * w1 + src.at<_tp>(i + x1, j + y2) * w2 \
                                 + src.at<_tp>(i + x2, j + y1) * w3 + src.at<_tp>(i + x2, j + y2) * w4;
                //LBP特征图像的每个邻居的LBP值累加，累加通过与操作完成，对应的LBP值通过移位取得
                dst.at<uchar>(i - radius, j - radius) |= (neighbor > center) << (neighbors - k - 1);
            }
        }
    }
}

Mat QImageToMat(QImage image)
{
    Mat mat;
    switch (image.format()) {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = Mat(image.height(), image.width(), CV_8UC4, (void *)image.constBits(), static_cast<size_t>(image.bytesPerLine()));
        break;
    case QImage::Format_RGB888:
        mat = Mat(image.height(), image.width(), CV_8UC3, (void *)image.constBits(), static_cast<size_t>(image.bytesPerLine()));
        cvtColor(mat, mat, COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = Mat(image.height(), image.width(), CV_8UC1, (void *)image.constBits(), static_cast<size_t>(image.bytesPerLine()));
        break;
    default:
        break;
    }

    return mat;
}
QImage MatToQImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1) {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++) {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row ++) {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, static_cast<size_t>(mat.cols));
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3) {
        // Copy input Mat
        const uchar *pSrc = static_cast<const uchar *>(mat.data);
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    } else if (mat.type() == CV_8UC4) {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);
        return image.copy();
    } else {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void RuiHua(Mat src)
{
    using namespace cv;
    Mat dst;
    int kernel_size = 1;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Mat imageEnhance;
    Mat kernel = (Mat_<float>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
    filter2D(src, imageEnhance, CV_8UC3, kernel);

    //    GaussianBlur( src, src, Size(3, 3), 0, 0, BORDER_DEFAULT );
    //    cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to grayscale

    Mat abs_dst;
    Laplacian( src, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );

    // converting back to CV_8U
    convertScaleAbs( dst, abs_dst );
    imshow("ruihua", abs_dst);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkMat()
{
    return !m_mat.empty();
}

void MainWindow::verfilter(int sigema)
{
    using namespace cv;
    Size s(1, sigema);
    Mat outMat;
    GaussianBlur(m_mat, outMat, s, 0, sigema);
    imshow("verfilter", outMat);
}

void MainWindow::horfilter(int sigema)
{
    using namespace cv;
    Size s(sigema, 1);
    Mat outMat;
    GaussianBlur(m_mat, outMat, s, sigema, 0);
    imshow("horfilter", outMat);
}

void MainWindow::gauss(int sigema)
{
    using namespace cv;
    Size s(sigema, sigema);
    Mat outMat1, imageEnhance;
    GaussianBlur(m_mat, outMat1, s, sigema);
//    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 4, 0, 0, -1, 0);
//    filter2D(outMat1, imageEnhance, CV_8UC3, kernel);
//    Laplacian(outMat1,outMat2,32);
    imshow("result", outMat1);
}

void MainWindow::sobel()
{
    using namespace cv;
    Mat out;
    Mat src_gray, binmary, grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    cvtColor(m_mat, src_gray, COLOR_RGB2GRAY);
    Scharr(src_gray, grad_x, CV_16S, 1, 0);
    Scharr(src_gray, grad_y, CV_16S, 0, 1);
//    Sobel(src_gray, grad_x, CV_16S, 1, 0);
//    Sobel(src_gray, grad_y, CV_16S, 0, 1);
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);
//    Canny(src_gray, out, 0, 0);
    imshow("sobel", out);
}

void MainWindow::lbp()
{
    Mat out;
    getCircularLBPFeatureOptimization<float>(m_mat, out, 3, 8);
    imshow("normal", out);
}

void MainWindow::classicalfilter()
{
    Mat img;
    m_mat.copyTo(img);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            img.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(0.349 * img.at<Vec3b>(i, j)[2] + 0.686 * img.at<Vec3b>(i, j)[1] + 0.168 * img.at<Vec3b>(i, j)[0]); // green
            img.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(0.393 * img.at<Vec3b>(i, j)[2] + 0.769 * img.at<Vec3b>(i, j)[1] + 0.189 * img.at<Vec3b>(i, j)[0]); // red
            img.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(0.272 * img.at<Vec3b>(i, j)[2] + 0.534 * img.at<Vec3b>(i, j)[1] + 0.131 * img.at<Vec3b>(i, j)[0]); // blue
        }
    }
    imshow("oldfilter", img);
}

void MainWindow::disorderColor()
{
    Mat img;
    m_mat.copyTo(img);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            for (int k = 0; k < 3; k++) {
                img.at<Vec3b>(i, j)[k] = 255 - img.at<Vec3b>(i, j)[k];
            }
        }
    }
    imshow("disorderColor", img);
}

void MainWindow::simpleColor(int color)
{
    Mat img;
    m_mat.copyTo(img);
    switch (color) {
    case 0: {
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                img.at<Vec3b>(i, j)[1] = 0;// green
                img.at<Vec3b>(i, j)[2] = 0;// red
            }
        }
    }
    break;
    case 1: {
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                img.at<Vec3b>(i, j)[0] = 0;// blue
                img.at<Vec3b>(i, j)[2] = 0;// red
            }
        }
    }
    break;
    case 2: {
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                img.at<Vec3b>(i, j)[0] = 0;// blue
                img.at<Vec3b>(i, j)[1] = 0;// green
            }
        }
    }
    break;
    default:
        break;
    }
    imshow("simpleColor", img);
}



void MainWindow::on_pushButton_clicked()
{
    QFileDialog d;
    d.exec();
    QString res = d.selectedFiles().first();
    m_mat = imread(res.toStdString());
    ui->label->setText(res);
    imshow("import", m_mat);
}

void MainWindow::on_pushButton_2_clicked()
{
    horfilter(15);
}

void MainWindow::on_pushButton_3_clicked()
{
    verfilter(15);
}

void MainWindow::on_pushButton_4_clicked()
{
    sobel();
}

void MainWindow::on_pushButton_5_clicked()
{
    HistogramND hist;
    hist.setImage(m_mat);
    hist.splitChannels();
    hist.getHistogram();
    hist.displayHisttogram();
}

void MainWindow::on_pushButton_6_clicked()
{
    Mat structureElement = getStructuringElement(MORPH_RECT, //矩形
                                                 Size(3, 3),//核的尺寸
                                                 Point(-1, -1));//锚点的位置
    Mat out_dilate, out_erode;
    dilate(m_mat, out_dilate, structureElement);
    erode(m_mat, out_erode, structureElement);
    imshow("dilate", out_dilate);
    imshow("erode", out_erode);
}

void MainWindow::on_pushButton_7_clicked()
{
    gauss(15);
}

void MainWindow::on_pushButton_8_clicked()
{
    lbp();
}

void MainWindow::on_pushButton_9_clicked()
{
    disorderColor();
}

void MainWindow::on_pushButton_10_clicked()
{
    simpleColor(1);
}

void MainWindow::on_pushButton_11_clicked()
{
    classicalfilter();
}
