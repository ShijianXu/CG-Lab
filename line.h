#ifndef LINE_H
#define LINE_H
#include "shape.h"

class Line : public Shape
{
public:
    Line();

    void paint(QPainter &painter);
    int slope(QPoint a, QPoint b);
    void fillShape(QPainter &painter);
    void updateFillPoint(int dx, int dy);
};

#endif // LINE_H
