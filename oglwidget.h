#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();
    QTimer *timer;

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    //void resizeGL(int w, int h) Q_DECL_OVERRIDE;
};

#endif // OGLWIDGET_H
