#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
//#include <QOpenGLWidget>
//#include <QOpenGLFunctions_4_3_Compatibility>

#include "shape.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "polygon.h"
#include "bezier.h"
#include "bspline.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);

signals:
    //void sendFillPolygon();

public slots:
    void setCurrentShape(Shape::Code s);
    void clearShapes();
    void fillPolygon();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void rotateClockwise();
    void rotateCounterClockwise();
    void ZoomIn();
    void ZoomOut();
    void trim();
    void savePic();
    void importPic();
    void saveShape();
    void importShape();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void changePoints(int dx, int dy);
    void zoomPoints(int type);
    void ShapeTrim();
    void LiangBarsky();
    void SutherlandHodgman();

private:
    Shape::Code currShapeCode;
    Shape *shape;
    bool painted;
    bool fill;
    bool solid; //判断对象是否实例化
    bool selectPoint; //选中某个点
    bool selectStart;
    bool selectEnd;
    int pointIndex; //选中点的下标
    bool pixNull;
    QPixmap pixmap;
//    bool perm;
    QList<Shape*> shapeList;
    bool trimWin;
    QPoint wins;
    QPoint wine;
};

#endif // PAINTWIDGET_H
