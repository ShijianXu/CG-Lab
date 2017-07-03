#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "shape.h"
#include "paintwidget.h"
#include "oglwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    OGLWidget *w;
    ~MainWindow();

signals:
    void changeCurrentShape(Shape::Code newShape);
    void sendClearSignal();
    void fill();
    void left();
    void right();
    void up();
    void down();
    void rotateR();
    void rotateL();
    void zoomin();
    void zoomout();
    void cut();
    void bezierCurve();
    void bsplineCurve();
    void save_pic();
    void import_pic();
    void save_shape();
    void import_shape();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_pressed();

    void on_pushButton_6_pressed();

    void on_pushButton_9_pressed();

    void on_pushButton_10_pressed();

    void on_pushButton_11_pressed();

    void on_pushButton_12_pressed();

    void on_pushButton_13_pressed();

    void on_pushButton_14_pressed();

    void on_pushButton_15_clicked();

    void on_bezier_clicked();

    void on_bspline_clicked();

    void on_savepic_clicked();

    void on_importpic_clicked();

    void on_saveshape_clicked();

    void on_importshape_clicked();

    void on_cube_released();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
