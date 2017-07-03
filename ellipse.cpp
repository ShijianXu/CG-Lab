#include "ellipse.h"
#include <cmath>

Ellipse::Ellipse()
{

}

void Ellipse::updateFillPoint(int dx, int dy)
{

}

void Ellipse::fillShape(QPainter &painter)
{
}

void Ellipse::paint(QPainter &painter)
{
    painter.setPen(Qt::black);
    painter.drawRect(start.x()-2, start.y()-2, 4, 4);
    painter.drawRect(end.x()-2, end.y()-2, 4, 4);

    int xa = start.x();
    int ya = start.y();
    int xb = end.x();
    int yb = end.y();

    int xc = (xa+xb)/2;
    int yc = (ya+yb)/2;
    int rx = abs(xa-xb)/2;
    int ry = abs(ya-yb)/2;


    int xn = 0; int yn = ry;
    int slope = 2*ry*ry*xn-2*rx*rx*yn;
    int x2 = xn; int y2 = -yn;
    int x3 = -x2; int y3 = y2;
    int x4 = x3; int y4 = -y3;
    painter.drawPoint(xn+xc,yn+yc);
    painter.drawPoint(x2+xc,y2+yc);
    painter.drawPoint(x3+xc,y3+yc);
    painter.drawPoint(x4+xc,y4+yc);

    double p0 = ry*ry - rx*rx*ry + (rx*rx)/4;

    while(slope<0)
    {
        if(p0<0)
        {
            xn+=1;
            yn=yn;
            x2 = xn; y2 = -yn;
            x3 = -x2; y3 = y2;
            x4 = x3; y4 = -y3;
            painter.drawPoint(xn+xc,yn+yc);
            painter.drawPoint(x2+xc,y2+yc);
            painter.drawPoint(x3+xc,y3+yc);
            painter.drawPoint(x4+xc,y4+yc);
            p0+=2*ry*ry*xn+ry*ry;
            slope = 2*ry*ry*xn-2*rx*rx*yn;
        }
        else
        {
            xn+=1;
            yn-=1;
            x2 = xn; y2 = -yn;
            x3 = -x2; y3 = y2;
            x4 = x3; y4 = -y3;
            painter.drawPoint(xn+xc,yn+yc);
            painter.drawPoint(x2+xc,y2+yc);
            painter.drawPoint(x3+xc,y3+yc);
            painter.drawPoint(x4+xc,y4+yc);
            p0+=2*ry*ry*xn-2*rx*rx*yn+ry*ry;
            slope = 2*ry*ry*xn-2*rx*rx*yn;
        }
    }
    //p0 = ry*ry*(xn+1/2)*(xn+1/2) + rx*rx*(yn-1) - rx*rx*ry*ry;
    p0 = 2*ry*(xn+1/2) + 2*rx*(yn-1) - 2*rx*ry;
    //double d = (b * (xn + 0.5)) * 2 + (a * (yn - 1)) * 2 - (a * b) * 2;
    while(yn!=0)
    {
        if(p0>0)
        {
            xn = xn;
            yn -= 1;
            x2 = xn; y2 = -yn;
            x3 = -x2; y3 = y2;
            x4 = x3; y4 = -y3;
            painter.drawPoint(xn+xc,yn+yc);
            painter.drawPoint(x2+xc,y2+yc);
            painter.drawPoint(x3+xc,y3+yc);
            painter.drawPoint(x4+xc,y4+yc);
            p0+=-2*rx*rx*yn + rx*rx;
            //d += sqa * (-2 * y + 3);
        }
        else
        {
            xn+=1;
            yn-=1;
            x2 = xn; y2 = -yn;
            x3 = -x2; y3 = y2;
            x4 = x3; y4 = -y3;
            painter.drawPoint(xn+xc,yn+yc);
            painter.drawPoint(x2+xc,y2+yc);
            painter.drawPoint(x3+xc,y3+yc);
            painter.drawPoint(x4+xc,y4+yc);

            p0+= 2*ry*ry*xn - 2*rx*rx*yn + rx*rx;
        }
    }
}
