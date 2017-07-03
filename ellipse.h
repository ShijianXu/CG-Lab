#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "shape.h"

class Ellipse : public Shape
{
public:
    Ellipse();
    void paint(QPainter &painter);
    void fillShape(QPainter &painter);
    void updateFillPoint(int dx, int dy);
};

#endif // ELLIPSE_H
