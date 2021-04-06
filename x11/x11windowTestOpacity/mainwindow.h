#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QWindow>

struct strucWindow {
    int wid = 0;
    QString name = "";
    QWindow *window{nullptr};
    double opacity{1.0};
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setIdTransparent(const QString &name);
    void setNameTransparent(const QString &id);

    void updateTableWidget();
private slots:
    void on_slider_valueChanged(int value);
    void on_idBtn_clicked();
    void on_nameBtn_clicked();


    void on_pushButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);

private:
    QWindow *m_currentWidnow{nullptr};
    Ui::MainWindow *ui;
    QList <QWindow *> m_windowList;
    QMap <int, strucWindow> m_windowVec;
};

#endif // MAINWINDOW_H
