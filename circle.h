#ifndef CIRCLE_H
#define CIRCLE_H
#include "shape.h"

class Circle : public Shape
{
public:
    Circle();
    void paint(QPainter &painter);
    void fillShape(QPainter &painter);
    void updateFillPoint(int dx, int dy);
};

#endif // CIRCLE_H
