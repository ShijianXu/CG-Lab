#ifndef POLYGON_H
#define POLYGON_H
#include "shape.h"
#include <QObject>
#include <QVector>

struct Edge
{
    QPoint a, b;
};

struct Node
{
    int ymax;       //边的最大y值
    double x;       //有序边表中，记录边下端点的x值；活化边表中，记录交点x值
    double slop_t;  //边斜率倒数
};

class Polygon : public Shape
{       
 //   Q_OBJECT
public:
    Polygon();

    void paint(QPainter &painter);
    int slope(QPoint a, QPoint b);
    void draw(QPainter &painter, QPoint s, QPoint e);
    void fillShape(QPainter &painter);
    void updateFillPoint(int dx, int dy);
private:
    QList<Edge> edge;
    QList<Node> net[1000];
    QList<Node> aet;
    //QList<QPoint> colorPoint;
};

#endif // POLYGON_H
