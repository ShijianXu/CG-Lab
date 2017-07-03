#include "polygon.h"
#include <cmath>
#include <stdio.h>
#include <algorithm>

Polygon::Polygon()
{

}

//slope return type:
//0: point
//1: vertical
//2: horizal
//3,4,5,6: normal slope exits

void Polygon::updateFillPoint(int dx, int dy)
{
        QList<QPoint> list;
        foreach(QPoint p, colorPoint)
        {
            QPoint temp;
            temp.setX(p.x()+dx*8);
            temp.setY(p.y()+dy*8);
            list.append(temp);
            colorPoint.removeFirst();
        }
        colorPoint.clear();
        foreach(QPoint p, list)
        {
            QPoint temp = p;
            colorPoint.append(temp);
        }
}

bool operate(const Node &n1, const Node &n2)
{
  /*  if(n1.x==n2.x)
        return n1.slop_t<n2.slop_t;
    else*/
    return n1.x<n2.x;
}

void Polygon::fillShape(QPainter &painter)
{
    if(colorPoint.length()!=0)
        return;
    //计算最高y值
    int maxy = 0;
    int miny = 500;
    for(int i =0; i<pointList.length();i++)
    {
        if(pointList[i].y() > maxy)
            maxy = pointList[i].y();
        if(pointList[i].y() < miny)
            miny = pointList[i].y();
    }

    for(int i=0;i<1000;i++)
    {
        net[i].clear();
    }
    aet.clear();

    //建立NET有序边表
    int len = pointList.length();
    for(int i = 0; i < len;i++)
    {
//        QPoint a0 = pointList[(i-1+len)%len];
        QPoint a  = pointList[i];
        QPoint b  = pointList[(i+1)%len];
//        QPoint b0 = pointList[(i+2)%len];

        if(a.y()==b.y())
            continue;

        //一条边的上下端y值
        int ymin = a.y()<b.y()?a.y():b.y();
        int ymax = a.y()>b.y()?a.y():b.y();

        float x = a.y()>b.y()?b.x():a.x();
        float dx = (a.x()-b.x())*1.0/(a.y()-b.y());

/*        if((a.y()<b.y() && a.y()>a0.y()) ||
           (b.y()<a.y() && b.y()>b0.y()) )
        {
            ymin++;
            x+=dx;
        }*/

        //创建边，加入有序边表
        Node t;
        t.ymax = ymax;
        t.x = x;
        t.slop_t = dx;
        net[ymin].append(t);
    }

    //扫描线
    for(int i = 0; i < maxy; i++)
    {
//        if(net[i].length()!=0)
//            printf("start point edges num %d\n", net[i].length());

        for(int j = 0; j < net[i].length();j++)
        {
            Node t = net[i][j];
            aet.append(t);
        }
        std::sort(aet.begin(),aet.end(), operate);

        //pop ymax==i
        for(int j = 0; j < aet.length();j++)
        {
            if(aet[j].ymax == i)
            {
                aet.removeAt(j);
                //BUG，删除之后后面的整体往前移动，如果j不减一，会丢失对一个节点的检查
                j=j-1;
            }
        }

//        printf("cross node num %d\n", aet.length());

        //draw
        int k = 0;
        while(k<aet.length())
        {
            int x1 = aet[k].x;
            int x2;
            if(k+1<aet.length())
                x2 = aet[k+1].x;
            else
                break;

            for(int ll = x1+1;ll<=x2;ll+=1)
            {
                painter.drawPoint(ll,i);
                QPoint temp;
                temp.setX(ll);
                temp.setY(i);
                colorPoint.append(temp);
            }
            k+=2;
        }

        for(int j = 0; j < aet.length();j++)
        {
            aet[j].x+=aet[j].slop_t;
        }
    }

}

int Polygon::slope(QPoint a, QPoint b)
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

void Polygon::draw(QPainter &painter, QPoint s, QPoint e)
{
    int xa = s.x();
    int ya = s.y();
    int xb = e.x();
    int yb = e.y();
    int dx = abs(xa-xb);
    int dy = abs(ya-yb);

    int slope_type = slope(s,e);
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

void Polygon::paint(QPainter &painter)
{
    painter.setPen(Qt::cyan);
    foreach (QPoint p, pointList) {
        painter.drawRect(p.x()-2, p.y()-2, 4, 4);
    }

    int n = 0;
    if ((n=pointList.length())<2)
        return;

    int len = pointList.length();
    if(len < 3)
    {
        return;
    }
    //将边的信息提取出来
    QPoint s = pointList[0];
    QPoint e = pointList[1];

    draw(painter, s, e);

    for(int i = 2; i < len; i++)
    {
        s = e; e = pointList[i];
        draw(painter, s, e);
    }
    s = e; e = pointList[0];
    draw(painter, s, e);

    foreach(QPoint p, colorPoint)
    {
        painter.drawPoint(p.x(),p.y());
    }

}
