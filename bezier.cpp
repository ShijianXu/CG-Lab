#include "bezier.h"

Bezier::Bezier()
{

}

void Bezier::updateFillPoint(int dx, int dy)
{

}

void Bezier::fillShape(QPainter &painter)
{
}

void Bezier::paint(QPainter &painter)
{
    QList<QPoint> list;
    painter.setPen(Qt::darkMagenta);
    foreach (QPoint p, pointList) {
        painter.drawRect(p.x()-2, p.y()-2, 4, 4);
    }

    int n = 0;
    if ((n=pointList.length())<2)
        return;

    double *xarray = new double[n - 1];
    double *yarray = new double[n - 1];
    double x = pointList[0].x();
    double y = pointList[0].y();
    for(double t = 0.0; t <=1; t += 0.001 / n) // 调整参数t,计算贝塞尔曲线上的点的坐标,t即为上述u
    {
        for(int i = 1; i < n; ++i)
        {
            for(int j = 0; j < n - i; ++j)
            {
                if(i == 1) // i==1时,第一次迭代,由已知控制点计算
                {
                    xarray[j] = pointList[j].x() * (1 - t) + pointList[j + 1].x() * t;
                    yarray[j] = pointList[j].y() * (1 - t) + pointList[j + 1].y() * t;
                    continue;
                }
                // i != 1时,通过上一次迭代的结果计算
                xarray[j] = xarray[j] * (1 - t) + xarray[j + 1] * t;
                yarray[j] = yarray[j] * (1 - t) + yarray[j + 1] * t;
            }
        }
        x = xarray[0];
        y = yarray[0];
        QPoint p;
        p.setX(x);
        p.setY(y);
        list.append(p);
    }
    delete []xarray;
    delete []yarray;
    foreach(QPoint p, list)
    {
        painter.drawPoint(p.x(),p.y());
    }
}
