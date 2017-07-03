#ifndef BSPLINE_H
#define BSPLINE_H
#include "shape.h"

class BSpline : public Shape
{
public:
    BSpline();
    void paint(QPainter &painter);
    void fillShape(QPainter &painter);
    void updateFillPoint(int dx, int dy);
};

#endif // BSPLINE_H
