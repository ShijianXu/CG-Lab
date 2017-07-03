#ifndef BEZIER_H
#define BEZIER_H
#include "shape.h"

class Bezier : public Shape
{
public:
    Bezier();
    void paint(QPainter &painter);
    void fillShape(QPainter &painter);
    void updateFillPoint(int dx, int dy);
};

#endif // BEZIER_H
