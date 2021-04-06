#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage image(64, 64, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setOpacity(0);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRect(0, 0, 64, 64);
    image.save("/home/lmh/Desktop/mycx/1.png", "PNG");
}

MainWindow::~MainWindow()
{
    delete ui;
}
