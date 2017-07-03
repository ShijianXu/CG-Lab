#include "oglwidget.h"
#include "glut.h"

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

double xrot = 0, yrot = 0, zrot =0;
void OGLWidget::paintGL()
{
    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);


    glEnable(GL_DEPTH_TEST);
    gluLookAt(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0);
    glRotatef(xrot, 1, 0, 0);
    glRotatef(yrot, 0, 1, 0);
    glRotatef(zrot, 0, 0, 1);
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);                    //绿色
            glVertex3f(0.25f, 0.25f, -0.25f);                  //右上(顶面)
            glVertex3f(-0.25f, 0.25f, -0.25f);                 //左上(顶面)
            glVertex3f(-0.25f, 0.25f, 0.25f);                  //左下(顶面)
            glVertex3f(0.25f, 0.25f, 0.25f);                   //右下(顶面)

            glColor3f(1.0f, 0.5f, 0.0f);                    //橙色
            glVertex3f(0.25f, -0.25f, 0.25f);                  //右上(底面)
            glVertex3f(-0.25f, -0.25f, 0.25f);                 //左上(底面)
            glVertex3f(-0.25f, -0.25f, -0.25f);                //左下(底面)
            glVertex3f(0.25f, -0.25f, -0.25f);                 //右下(底面)

            glColor3f(1.0f, 0.0f, 0.0f);                    //红色
            glVertex3f(0.25f, 0.25f, 0.25f);                   //右上(前面)
            glVertex3f(-0.25f, 0.25f, 0.25f);                  //左上(前面)
            glVertex3f(-0.25f, -0.25f, 0.25f);                 //左下(前面)
            glVertex3f(0.25f, -0.25f, 0.25f);                  //右下(前面)

            glColor3f(1.0f, 1.0f, 0.0f);                    //黄色
            glVertex3f(0.25f, -0.25f, -0.25f);                 //右上(后面)
            glVertex3f(-0.25f, -0.25f, -0.25f);                //左上(后面)
            glVertex3f(-0.25f, 0.25f, -0.25f);                 //左下(后面)
            glVertex3f(0.25f, 0.25f, -0.25f);                  //右下(后面)

            glColor3f(0.0f, 0.0f, 1.0f);                    //蓝色
            glVertex3f(-0.25f, 0.25f, 0.25f);                  //右上(左面)
            glVertex3f(-0.25f, 0.25f, -0.25f);                 //左上(左面)
            glVertex3f(-0.25f, -0.25f, -0.25f);                //左下(左面)
            glVertex3f(-0.25f, -0.25f, 0.25f);                 //右下(左面)

            glColor3f(1.0f, 0.0f, 1.0f);                    //紫色
            glVertex3f(0.25f, 0.25f, -0.25f);                  //右上(右面)
            glVertex3f(0.25f, 0.25f, 0.25f);                   //左上(右面)
            glVertex3f(0.25f, -0.25f, 0.25f);                  //左下(右面)
            glVertex3f(0.25f, -0.25f, -0.25f);                 //右下(右面)
    glEnd();

    xrot += 1;
    yrot += 1;
    zrot += 1;
}
