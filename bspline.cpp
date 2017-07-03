#include "bspline.h"
#include <iostream>

BSpline::BSpline()
{

}

void BSpline::updateFillPoint(int dx, int dy)
{

}

void BSpline::fillShape(QPainter &painter)
{
}

void BSpline::paint(QPainter &painter)
{
    painter.setPen(Qt::green);
    foreach (QPoint p, pointList) {
        painter.drawRect(p.x()-2, p.y()-2, 4, 4);
        //painter.drawPoint(p.x(),p.y());
    }
    /*
    int k = 4;      //默认为4阶B样条
    int n;
    n = pointList.length()-1;

    double *u = new double[n+k+1];
    for(int i = 0; i <= k-1; i ++)
        u[i] = 0.0;
    for(int i = n+1; i <= n+k; i ++)
        u[i] = 1.0;
    double delta = 1.0 / (double)(n+1-k+1);
    for(int i = k; i < n+1; i++)
        u[i] = u[i-1]+delta;*/

    int NUM_POINTS = pointList.length();
    if(NUM_POINTS < 3)
        return;

    int NUM_SEGMENTS = NUM_POINTS-3;

    int n = 1000;
    float f1, f2, f3, f4;
    float deltaT = 1.0 / n;
    float T;

    QList<QPoint> list;
    QList<int> temp;
    for (int num = 0; num < NUM_SEGMENTS; num++)
    {
        for (int i = 0; i <= n; i++)
        {
            T = i * deltaT;
            f1 = (-T*T*T + 3*T*T - 3*T + 1) / 6.0;
            f2 =(3*T*T*T - 6*T*T + 4) / 6.0;
            f3 = (-3*T*T*T +3*T*T + 3*T + 1) / 6.0;
            f4 = (T*T*T) / 6.0;

            QPoint p;
            double x = f1*pointList[num].x() + f2*pointList[num+1].x() + f3*pointList[num+2].x() + f4*pointList[num+3].x();
            double y = f1*pointList[num].y() + f2*pointList[num+1].y() + f3*pointList[num+2].y() + f4*pointList[num+3].y();
            p.setX(x);
            p.setY(y);
            list.append(p);
            if(num%2==0)
                temp.append(1);
            else
                temp.append(0);
        }
    }
    int count = 0;
    foreach(QPoint p, list)
    {
        if(temp[count]==0)
            painter.setPen(Qt::red);
        else
            painter.setPen(Qt::green);
        count++;
        painter.drawPoint(p.x(),p.y());
    }
}
