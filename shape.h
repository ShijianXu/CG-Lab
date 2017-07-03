#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QPainter>

class Shape
{
public:
    enum Code{None, Line,Rect,Circle,Ellipse, Polygon, Bezier, BSpline };
    Shape();

    void setStart(QPoint s) { start=s; }
    void setEnd(QPoint e) { end = e; }
    QPoint startPoint() { return start; }
    QPoint endPoint() { return end; }
    void appendPoint(QPoint p) { pointList.append(p); }
    void virtual paint(QPainter & painter)=0;
    void virtual fillShape(QPainter & painter)=0;
    void virtual updateFillPoint(int dx, int dy)=0;
    QList<QPoint> pointList;
    QList<QPoint> colorPoint;

protected:
    QPoint start;
    QPoint end;
};

#endif // SHAPE_H
