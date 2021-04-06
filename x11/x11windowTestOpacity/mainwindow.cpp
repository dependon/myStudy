#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWindow>
#include <QDebug>

#include "setdesktop.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    auto display = XOpenDisplay(nullptr);
//    auto root_window = DefaultRootWindow(display);

//    Window root_return, parent_return;
//    Window *child_list = nullptr;
//    unsigned int child_num = 0;
//    XQueryTree(display, root_window, &root_return, &parent_return, &child_list, &child_num);

//    for (unsigned int i = 0; i < child_num; ++i) {
//        Window window = child_list[i];
////        XWindowAttributes attrs;
////        XGetWindowAttributes(display, window, &attrs);

//        QWindow *windowq = QWindow::fromWinId((unsigned long)window);
//        if (windowq) {
//            windowq->setOpacity(0.8);
//        }
//        qDebug() << window;
//        m_windowList.push_back(windowq);

//    }
//    qDebug() << child_num;
//    XFree(child_list);
//    XCloseDisplay(display);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNameTransparent(const QString &name)
{
    char *str = NULL;
    QByteArray ba = name.toLatin1();
    str = (char *)malloc(ba.length() + 1);
    memset(str, 0, ba.length());
    memcpy(str, ba.data(), ba.length());
    str[ba.length()] = '\0';
    //设置desktop透明
    int pid_t[128];
    find_pid_by_name(str, pid_t);
    int pid = pid_t[0];
    Display *display = XOpenDisplay(0);
    WindowsMatchingPid match(display, XDefaultRootWindow(display), pid);
    const list<Window> &result = match.result();

    for (Window id : result) {
        QWindow *window = QWindow::fromWinId((unsigned long)id);
        if (window != nullptr) {

            int value = ui->slider->value();
            double a = (double)value;
            double o = a / 100.0;
            window->setOpacity(o);

            m_currentWidnow = window;
            strucWindow st;
            st.window = window;
            st.wid = id;
            st.name = name;
            st.opacity = o;
            m_windowVec.insert(id, st);

//            ui->listWidget->addItem(name + QString::number(id));

        }
    }
    updateTableWidget();
}

void MainWindow::updateTableWidget()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(m_windowVec.count());
    for (int index = 0; index < ui->tableWidget->rowCount(); index++) {
        strucWindow tmpst = m_windowVec[m_windowVec.keys().at(index)];
        QTableWidgetItem *itemWid = new QTableWidgetItem();
        itemWid->setText(QString::number(tmpst.wid));
        QTableWidgetItem *itemName = new QTableWidgetItem();
        itemName->setText(tmpst.name);
        QTableWidgetItem *itemOpacity = new QTableWidgetItem();
        itemOpacity->setText(QString::number(tmpst.opacity));
        ui->tableWidget->setItem(index, 0, itemWid);
        ui->tableWidget->setItem(index, 1, itemName);
        ui->tableWidget->setItem(index, 2, itemOpacity);
    }
}

void MainWindow::on_slider_valueChanged(int value)
{
//    if (m_currentWidnow) {
//        double a = (double)value;
//        double o = a / 100.0;
//        m_currentWidnow->setOpacity(o);
//    }
}

void MainWindow::setIdTransparent(const QString &id)
{

}

void MainWindow::on_idBtn_clicked()
{
    qDebug() << "test";
    QString id = ui->idEdit->text();

}

void MainWindow::on_nameBtn_clicked()
{
    qDebug() << "test";
    QString name = ui->nameEdit->text();

    setNameTransparent(name);
}

void MainWindow::on_pushButton_clicked()
{
    for (auto structwindow : m_windowVec) {
        if (structwindow.window) {
            int value = ui->slider->value();
            double a = (double)value;
            double o = a / 100.0;
            structwindow.window->setOpacity(o);
            structwindow.opacity = o;
            m_windowVec.insert(structwindow.wid, structwindow);
        }
    }
    updateTableWidget();
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (2 == column) {
        double opacity = ui->tableWidget->item(row, column)->text().toDouble();
        int key = ui->tableWidget->item(row, 0)->text().toInt();
        strucWindow stWindow = m_windowVec.value(key);
        if (stWindow.window) {
            stWindow.window->setOpacity(opacity);
            stWindow.opacity = opacity;
            m_windowVec.insert(stWindow.wid, stWindow);
        }
    }
}
