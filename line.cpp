#include "line.h"
#include <cmath>
#include <stdio.h>
Line::Line() {
}

void Line::updateFillPoint(int dx, int dy)
{

}

void Line::fillShape(QPainter &painter)
{
}

int Line::slope(QPoint a, QPoint b)
{
    int x1 = a.x();
    int y1 = a.y();
    int x2 = b.x();
    int y2 = b.y();

    int dx = x2-x1;
    int dy = y2-y1;

    if(x1==x2 && y1==y2)
        return 0;
    else if(x1==x2)
        return 1;
    else if(y1==y2)
        return 2;
    else
    {
        double s = (double)dy/(double)dx;
        if(s>0 && s<=1)
            return 3;
        else if(s>=-1 && s<0)
            return 4;
        else if(s>1)
            return 5;
        else
            return 6;
    }
}

void Line::paint(QPainter &painter)
{
    painter.setPen(QColor(0, 160, 230));
    painter.drawRect(start.x()-2, start.y()-2, 4, 4);
    painter.drawRect(end.x()-2, end.y()-2, 4, 4);


    int xa = start.x();
    int ya = start.y();
    int xb = end.x();
    int yb = end.y();

    int dx = abs(xa-xb);
    int dy = abs(ya-yb);

    int slope_type = slope(start,end);

    switch(slope_type)
    {
    case 0:
    {
        painter.drawPoint(xa,ya);
        break;
    }
    case 1:
    {
        int y1 = ya<yb?ya:yb;
        int y2 = ya>yb?ya:yb;
        for (int k=y1;k<=y2;k++)
            painter.drawPoint(xa,k);
        break;
    }
    case 2:
    {
        int x1 = xa<xb?xa:xb;
        int x2 = xa>xb?xa:xb;
        for (int k=x1;k<=x2;k++)
            painter.drawPoint(k,ya);
        break;
    }
    case 3:
    {
        int x1 = xa<xb?xa:xb;
        int y1 = ya<yb?ya:yb;
        int p0 = 2*dy-dx;

        painter.drawPoint(x1,y1);
        int xn = x1;
        int yn = y1;
        for (int i=1;i<=dx;i++)
        {
            if(p0<0)
            {
                xn+=1;
                yn=yn;
                painter.drawPoint(xn,yn);
                p0 +=2*dy;
            }
            else
            {
                xn+=1;
                yn+=1;
                painter.drawPoint(xn,yn);
                p0+=2*dy-2*dx;
            }
        }
        break;
    }
    case 4:
    {
        int x1=xa<xb?xa:xb;
        int y1=ya>yb?ya:yb;
        int p0 = 2*dy-dx;

        painter.drawPoint(x1,y1);
        int xn = x1;
        int yn = y1;
        for (int i=1;i<=dx;i++)
        {
            if(p0<0)
            {
                xn+=1;
                yn=yn;
                painter.drawPoint(xn,yn);
                p0 +=2*dy;
            }
            else
            {
                xn+=1;
                yn-=1;
                painter.drawPoint(xn,yn);
                p0+=2*dy-2*dx;
            }
        }
        break;
    }
    case 5:
    {
        int x1 = xa<xb?xa:xb;
        int y1 = ya<yb?ya:yb;
        int p0 = 2*dx-dy;

        painter.drawPoint(x1,y1);
        int xn = x1;
        int yn = y1;
        for (int i=1;i<=dy;i++)
        {
            if(p0<0)
            {
                yn+=1;
                xn=xn;
                painter.drawPoint(xn,yn);
                p0 +=2*dx;
            }
            else
            {
                yn+=1;
                xn+=1;
                painter.drawPoint(xn,yn);
                p0+=2*dx-2*dy;
            }
        }
        break;
    }
    case 6:
    {
        int x1=xa<xb?xa:xb;
        int y1=ya>yb?ya:yb;
        int p0 = 2*dx-dy;

        painter.drawPoint(x1,y1);
        int xn = x1;
        int yn = y1;
        for (int i=1;i<=dy;i++)
        {
            if(p0<0)
            {
                yn-=1;
                xn=xn;
                painter.drawPoint(xn,yn);
                p0 +=2*dx;
            }
            else
            {
                yn-=1;
                xn+=1;
                painter.drawPoint(xn,yn);
                p0+=2*dx-2*dy;
            }
        }
        break;
    }
    }
}
