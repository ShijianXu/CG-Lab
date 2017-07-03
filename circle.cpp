#include "circle.h"
#include <cmath>

Circle::Circle()
{

}
void Circle::updateFillPoint(int dx, int dy)
{

}

void Circle::fillShape(QPainter &painter)
{
}

void Circle::paint(QPainter &painter)
{
    painter.setPen(Qt::red);
    painter.drawRect(start.x()-2, start.y()-2, 4, 4);
    painter.drawRect(end.x()-2, end.y()-2, 4, 4);

    int xa = start.x();
    int ya = start.y();
    int xb = end.x();
    int yb = end.y();

    QPoint center;
    center.setX(int((xa+xb)/2));
    center.setY(int((ya+yb)/2));
    int radius = sqrt((xa-xb)*(xa-xb)+(ya-yb)*(ya-yb))/2;

    int xn=0; int yn=radius;
    int x2=yn; int y2=xn;
    int x3=x2; int y3=-y2;
    int x4=xn; int y4=-yn;
    int x5=-x4; int y5=y4;
    int x6=-x3; int y6=y3;
    int x7=x6; int y7=-y6;
    int x8=x5; int y8=-y5;
    painter.drawPoint(xn+center.x(), yn+center.y());
    painter.drawPoint(x2+center.x(),y2+center.y());
    painter.drawPoint(x3+center.x(),y3+center.y());
    painter.drawPoint(x4+center.x(),y4+center.y());
    painter.drawPoint(x5+center.x(),y5+center.y());
    painter.drawPoint(x6+center.x(),y6+center.y());
    painter.drawPoint(x7+center.x(),y7+center.y());
    painter.drawPoint(x8+center.x(),y8+center.y());
    double p0=5/4-radius;
    while(xn<yn)
    {
        if(p0<0)
        {
            xn+=1;
            yn=yn;
            p0 = p0+2*xn+1;
            x2=yn; y2=xn;
            x3=x2; y3=-y2;
            x4=xn; y4=-yn;
            x5=-x4; y5=y4;
            x6=-x3; y6=y3;
            x7=x6; y7=-y6;
            x8=x5; y8=-y5;
            painter.drawPoint(xn+center.x(), yn+center.y());
            painter.drawPoint(x2+center.x(),y2+center.y());
            painter.drawPoint(x3+center.x(),y3+center.y());
            painter.drawPoint(x4+center.x(),y4+center.y());
            painter.drawPoint(x5+center.x(),y5+center.y());
            painter.drawPoint(x6+center.x(),y6+center.y());
            painter.drawPoint(x7+center.x(),y7+center.y());
            painter.drawPoint(x8+center.x(),y8+center.y());
        }
        else
        {
            xn+=1;
            yn-=1;
            p0=p0+2*xn+1-2*yn;
            x2=yn; y2=xn;
            x3=x2; y3=-y2;
            x4=xn; y4=-yn;
            x5=-x4; y5=y4;
            x6=-x3; y6=y3;
            x7=x6; y7=-y6;
            x8=x5; y8=-y5;
            painter.drawPoint(xn+center.x(), yn+center.y());
            painter.drawPoint(x2+center.x(),y2+center.y());
            painter.drawPoint(x3+center.x(),y3+center.y());
            painter.drawPoint(x4+center.x(),y4+center.y());
            painter.drawPoint(x5+center.x(),y5+center.y());
            painter.drawPoint(x6+center.x(),y6+center.y());
            painter.drawPoint(x7+center.x(),y7+center.y());
            painter.drawPoint(x8+center.x(),y8+center.y());
        }
    }
}
