#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintwidget.h"
#include "oglwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PaintWidget *paintWidget = new PaintWidget(this);

    connect(this, SIGNAL(changeCurrentShape(Shape::Code)),paintWidget, SLOT(setCurrentShape(Shape::Code)));
    connect(this, SIGNAL(sendClearSignal()), paintWidget, SLOT(clearShapes()));
    connect(this, SIGNAL(fill()), paintWidget, SLOT(fillPolygon()));
    connect(this, SIGNAL(left()), paintWidget, SLOT(moveLeft()));
    connect(this, SIGNAL(right()), paintWidget, SLOT(moveRight()));
    connect(this, SIGNAL(up()), paintWidget, SLOT(moveUp()));
    connect(this, SIGNAL(down()), paintWidget, SLOT(moveDown()));
    connect(this, SIGNAL(rotateR()), paintWidget, SLOT(rotateClockwise()));
    connect(this, SIGNAL(rotateL()), paintWidget, SLOT(rotateCounterClockwise()));
    connect(this, SIGNAL(zoomin()), paintWidget, SLOT(ZoomIn()));
    connect(this, SIGNAL(zoomout()),paintWidget, SLOT(ZoomOut()));
    connect(this, SIGNAL(cut()), paintWidget, SLOT(trim()));
    connect(this, SIGNAL(save_pic()), paintWidget, SLOT(savePic()));
    connect(this, SIGNAL(import_pic()), paintWidget, SLOT(importPic()));
    connect(this, SIGNAL(save_shape()), paintWidget, SLOT(saveShape()));
    connect(this, SIGNAL(import_shape()), paintWidget, SLOT(importShape()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    emit changeCurrentShape(Shape::Line);
}

void MainWindow::on_pushButton_3_clicked()
{
    emit changeCurrentShape(Shape::Circle);
}

void MainWindow::on_pushButton_clicked()
{
    emit changeCurrentShape(Shape::Ellipse);
}

void MainWindow::on_pushButton_4_clicked()
{
    emit sendClearSignal();
}

void MainWindow::on_pushButton_5_clicked()
{
    emit changeCurrentShape(Shape::Polygon);
}

void MainWindow::on_pushButton_8_clicked()
{
    emit fill();
}

void MainWindow::on_pushButton_7_pressed()
{
    emit left();
}

void MainWindow::on_pushButton_6_pressed()
{
    emit right();
}

void MainWindow::on_pushButton_9_pressed()
{
    emit up();
}

void MainWindow::on_pushButton_10_pressed()
{
    emit down();
}

void MainWindow::on_pushButton_11_pressed()
{
    emit rotateR();
}

void MainWindow::on_pushButton_12_pressed()
{
    emit rotateL();
}

void MainWindow::on_pushButton_13_pressed()
{
    emit zoomin();
}

void MainWindow::on_pushButton_14_pressed()
{
    emit zoomout();
}

void MainWindow::on_pushButton_15_clicked()
{
    emit cut();
}

void MainWindow::on_bezier_clicked()
{
    emit changeCurrentShape(Shape::Bezier);
}

void MainWindow::on_bspline_clicked()
{
    emit changeCurrentShape(Shape::BSpline);
}

void MainWindow::on_savepic_clicked()
{
    emit save_pic();
}

void MainWindow::on_importpic_clicked()
{
    emit import_pic();
}

void MainWindow::on_saveshape_clicked()
{
    emit save_shape();
}

void MainWindow::on_importshape_clicked()
{
    emit import_shape();
}

void MainWindow::on_cube_released()
{
    w = new OGLWidget;
    w->show();
}
