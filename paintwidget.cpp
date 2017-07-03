#include "paintwidget.h"
#include <QMouseEvent>
#include <algorithm>
#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent),
    currShapeCode(Shape::Line), shape(NULL), painted(true), fill(false), solid(false), selectPoint(false), pixNull(true),
    selectStart(false), selectEnd(false)
{
    setMinimumSize(700,500);
    setMaximumSize(700,500);
}

void PaintWidget::zoomPoints(int type)
{
    if(currShapeCode==Shape::Line || currShapeCode==Shape::Circle || currShapeCode==Shape::Ellipse)
    {
        QPoint s,e;
        QPoint ts = shape->startPoint();
        QPoint te = shape->endPoint();
        double xc = (ts.x()+te.x())*1.0/2;
        double yc = (ts.y()+te.y())*1.0/2;

        if(type == 1)
        {
            double ssx = (float)ts.x() * 1.1;
            double ssy = (float)ts.y() * 1.1;
            double eex = (float)te.x() * 1.1;
            double eey = (float)te.y() * 1.1;
            double xxc = (ssx+eex)/2;
            double yyc = (ssy+eey)/2;
            double dx = -(xxc-xc);
            double dy = -(yyc-yc);

            s.setX(ssx+dx);
            s.setY(ssy+dy);
            e.setX(eex+dx);
            e.setY(eey+dy);
        }
        else
        {
            double ssx = (float)ts.x() * 0.9;
            double ssy = (float)ts.y() * 0.9;
            double eex = (float)te.x() * 0.9;
            double eey = (float)te.y() * 0.9;
            double xxc = (ssx+eex)/2;
            double yyc = (ssy+eey)/2;
            double dx = -(xxc-xc);
            double dy = -(yyc-yc);

            s.setX(ssx+dx);
            s.setY(ssy+dy);
            e.setX(eex+dx);
            e.setY(eey+dy);
        }
        shape->setStart(s);
        shape->setEnd(e);
        update();
    }
    else if(currShapeCode==Shape::Polygon || currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline)
    {
        int minx,miny,maxx,maxy;
        minx=5000;miny=5000;maxx=-1000;maxy=-1000;
        foreach(QPoint p, shape->pointList)
        {
            if(p.x()<minx)
                minx=p.x();
            if(p.y()<miny)
                miny=p.y();
            if(p.x()>maxx)
                maxx=p.x();
            if(p.y()>maxy)
                maxy=p.y();
        }
        double xc = (minx+maxx)*1.0/2;
        double yc = (miny+maxy)*1.0/2;

        QList<QPoint> list;
        foreach(QPoint p, shape->pointList)
        {
            QPoint temp;
            if(type == 1)
            {
                temp.setX((float)p.x()*1.1);
                temp.setY((float)p.y()*1.1);
            }
            else
            {
                temp.setX((float)p.x()*0.9);
                temp.setY((float)p.y()*0.9);
            }
            list.append(temp);
        }

        int minxx,minyy,maxxx,maxyy;
        minxx=5000;minyy=5000;maxxx=-1000;maxyy=-1000;
        foreach(QPoint p, list)
        {
            if(p.x()<minxx)
                minxx=p.x();
            if(p.y()<minyy)
                minyy=p.y();
            if(p.x()>maxxx)
                maxxx=p.x();
            if(p.y()>maxyy)
                maxyy=p.y();
        }
        double xxc = (minxx+maxxx)*1.0/2;
        double yyc = (minyy+maxyy)*1.0/2;

        double dx = -(xxc-xc);
        double dy = -(yyc-yc);

        QList<QPoint> list2;
        foreach(QPoint p, list)
        {
            QPoint temp;
            temp.setX(p.x()+dx);
            temp.setY(p.y()+dy);

            if(currShapeCode==Shape::Polygon && temp.y()<0)
                return;

            list2.append(temp);
        }

        shape->pointList.clear();

        foreach(QPoint p, list2)
        {
            QPoint temp = p;
            shape->pointList.append(temp);
        }
        shape->colorPoint.clear();
        update();
    }
}

void PaintWidget::changePoints(int dx, int dy)
{
    if(currShapeCode==Shape::Line || currShapeCode==Shape::Circle || currShapeCode==Shape::Ellipse)
    {
        QPoint s,e;
        QPoint ts = shape->startPoint();
        QPoint te = shape->endPoint();
        s.setX(ts.x()+dx*8);
        s.setY(ts.y()+dy*8);
        e.setX(te.x()+dx*8);
        e.setY(te.y()+dy*8);
        shape->setStart(s);
        shape->setEnd(e);
        update();
    }
    else if(currShapeCode==Shape::Polygon || currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline)
    {
        QList<QPoint> list;
        foreach(QPoint p, shape->pointList)
        {
            QPoint temp;
            temp.setX(p.x()+dx*8);
            temp.setY(p.y()+dy*8);

            if(currShapeCode==Shape::Polygon && temp.y()<0)
                return;

            list.append(temp);
        }
        shape->pointList.clear();
        foreach(QPoint p, list)
        {
            QPoint temp = p;
            shape->pointList.append(temp);
        }
        shape->updateFillPoint(dx,dy);

        update();
    }
}

void PaintWidget::rotateClockwise()
{
    if(currShapeCode==Shape::Line)   //|| currShapeCode==Shape::Circle || currShapeCode==Shape::Ellipse))
    {
        QPoint ts = shape->startPoint();
        QPoint te = shape->endPoint();

        double xc = (ts.x()+te.x())*1.0/2;
        double yc = (ts.y()+te.y())*1.0/2;

        double startx,starty,endx,endy;
        startx =ts.x()*1.0-xc;
        starty =ts.y()*1.0-yc;
        endx = te.x()*1.0-xc;
        endy = te.y()*1.0-yc;

        QPoint ss,ee;
        double xp = startx;
        double yp = -starty;

        double sx = cos(0.1)*xp+sin(0.1)*yp + xc;
        double sy = -(-sin(0.1)*xp+cos(0.1)*yp) + yc;
        ss.setX(ceil(sx));
        ss.setY(ceil(sy));

        xp = endx;
        yp = -endy;

        double ex = cos(0.1)*xp+sin(0.1)*yp + xc;
        double ey = -(-sin(0.1)*xp+cos(0.1)*yp) + yc;
        ee.setX(ceil(ex));
        ee.setY(ceil(ey));

        shape->setStart(ss);
        shape->setEnd(ee);

        update();
    }
    else if(currShapeCode==Shape::Polygon || currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline)
    {
        int minx,miny,maxx,maxy;
        minx=5000;miny=5000;maxx=-1000;maxy=-1000;
        foreach(QPoint p, shape->pointList)
        {
            if(p.x()<minx)
                minx=p.x();
            if(p.y()<miny)
                miny=p.y();
            if(p.x()>maxx)
                maxx=p.x();
            if(p.y()>maxy)
                maxy=p.y();
        }

        double xc = (minx+maxx)*1.0/2;
        double yc = (miny+maxy)*1.0/2;

        QList<QPoint> list;
        foreach(QPoint p, shape->pointList)
        {

            double x = p.x()-xc;
            double y = -(p.y()-yc);

            double xp = cos(0.1)*x+sin(0.1)*y + xc;
            double yp = -(-sin(0.1)*x+cos(0.1)*y) + yc;

            p.setX(ceil(xp));
            p.setY(ceil(yp));

            if(currShapeCode==Shape::Polygon && p.y()<0)
                return;

            QPoint temp = p;
            list.append(temp);
        }
        shape->pointList.clear();
        foreach(QPoint p, list)
        {
            shape->pointList.append(p);
        }
        if(currShapeCode==Shape::Polygon && shape->colorPoint.length()!=0)
            fill=true;
        shape->colorPoint.clear();
        update();
    }
}

void PaintWidget::rotateCounterClockwise()
{
    if(currShapeCode==Shape::Line)   //|| currShapeCode==Shape::Circle || currShapeCode==Shape::Ellipse))
    {
        QPoint ts = shape->startPoint();
        QPoint te = shape->endPoint();

        double xc = (ts.x()+te.x())*1.0/2;
        double yc = (ts.y()+te.y())*1.0/2;

        double startx,starty,endx,endy;
        startx =ts.x()*1.0-xc;
        starty =ts.y()*1.0-yc;
        endx = te.x()*1.0-xc;
        endy = te.y()*1.0-yc;

        QPoint ss,ee;
        double xp = startx;
        double yp = -starty;

        double sx = cos(0.1)*xp-sin(0.1)*yp + xc;
        double sy = -(sin(0.1)*xp+cos(0.1)*yp) + yc;
        ss.setX(floor(sx));
        ss.setY(floor(sy));

        xp = endx;
        yp = -endy;

        double ex = cos(0.1)*xp-sin(0.1)*yp + xc;
        double ey = -(sin(0.1)*xp+cos(0.1)*yp) + yc;
        ee.setX(floor(ex));
        ee.setY(floor(ey));

        shape->setStart(ss);
        shape->setEnd(ee);

        update();
    }
    else if(currShapeCode==Shape::Polygon || currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline)
    {
        int minx,miny,maxx,maxy;
        minx=1000;miny=1000;maxx=-1000;maxy=-1000;
        foreach(QPoint p, shape->pointList)
        {
            if(p.x()<minx)
                minx=p.x();
            if(p.y()<miny)
                miny=p.y();
            if(p.x()>maxx)
                maxx=p.x();
            if(p.y()>maxy)
                maxy=p.y();
        }

        double xc = (minx+maxx)*1.0/2;
        double yc = (miny+maxy)*1.0/2;

        QList<QPoint> list;
        foreach(QPoint p, shape->pointList)
        {

            double x = p.x()-xc;
            double y = -(p.y()-yc);

            double xp = cos(0.1)*x-sin(0.1)*y + xc;
            double yp = -(sin(0.1)*x+cos(0.1)*y) + yc;

            p.setX(floor(xp));
            p.setY(floor(yp));

            if(currShapeCode==Shape::Polygon && p.y()<0)
                return;
            QPoint temp = p;
            list.append(temp);
        }
        shape->pointList.clear();
        foreach(QPoint p, list)
        {
            shape->pointList.append(p);
        }
        if(currShapeCode==Shape::Polygon && shape->colorPoint.length()!=0)
            fill=true;
        shape->colorPoint.clear();
        update();
    }
}

void PaintWidget::moveLeft()
{
    changePoints(-1,0);
}

void PaintWidget::moveRight()
{
    changePoints(1,0);
}

void PaintWidget::moveUp()
{
    changePoints(0,-1);
}

void PaintWidget::moveDown()
{
    changePoints(0,1);
}

void PaintWidget::ZoomIn()
{
    zoomPoints(1);
}

void PaintWidget::ZoomOut()
{
    zoomPoints(-1);
}

void PaintWidget::trim()
{
    trimWin = true;
}

void PaintWidget::fillPolygon()
{
    fill = true;
    if(painted)
        update();
}

float maxi(float arr[],int n) {
  float m = 0;
  for (int i = 0; i < n; ++i)
    if (m < arr[i])
      m = arr[i];
  return m;
}

// this function gives the minimum
float mini(float arr[], int n) {
  float m = 1;
  for (int i = 0; i < n; ++i)
    if (m > arr[i])
      m = arr[i];
  return m;
}

void PaintWidget::LiangBarsky()
{
    float xmin = qMin(wins.x(),wine.x());
    float xmax = qMax(wins.x(),wine.x());
    float ymin = qMin(wins.y(),wine.y());
    float ymax = qMax(wins.y(),wine.y());
    QPoint s = shape->startPoint();
    QPoint e = shape->endPoint();

    float x1 = s.x();
    float x2 = e.x();
    float y1 = s.y();
    float y2 = e.y();

    float p1 = -(x2 - x1);
    float p2 = -p1;
    float p3 = -(y2 - y1);
    float p4 = -p3;

    float q1 = x1 - xmin;
    float q2 = xmax - x1;
    float q3 = y1 - ymin;
    float q4 = ymax - y1;

    float posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;

    if ((p1 == 0 && (q1 < 0 || q2 < 0)) || (p3 == 0 && (q3 < 0 || q4 < 0)))
    {
        shape = NULL;
        shapeList.removeLast();
        solid = false;
        return;
    }


    if(p1 != 0)
    {
        float r1 = q1 / p1;
        float r2 = q2 / p2;
        if (p1 < 0)
        {
            negarr[negind++] = r1; // for negative p1, add it to negative array
            posarr[posind++] = r2; // and add p2 to positive array
        }
        else
        {
            negarr[negind++] = r2;
            posarr[posind++] = r1;
        }
    }
    if (p3 != 0)
    {
        float r3 = q3 / p3;
        float r4 = q4 / p4;
        if (p3 < 0)
        {
            negarr[negind++] = r3;
            posarr[posind++] = r4;
        }
        else
        {
            negarr[negind++] = r4;
            posarr[posind++] = r3;
        }
    }

    float xn1, yn1, xn2, yn2;
    float rn1, rn2;
    rn1 = maxi(negarr, negind); // maximum of negative array
    rn2 = mini(posarr, posind); // minimum of positive array

    if(rn1>rn2)
    {
        shape = NULL;
        shapeList.removeLast();
        solid = false;
        return;
    }

    xn1 = x1 + p2 * rn1;
    yn1 = y1 + p4 * rn1; // computing new points

    xn2 = x1 + p2 * rn2;
    yn2 = y1 + p4 * rn2;

    QPoint t1,t2;
    t1.setX(xn1);
    t1.setY(yn1);
    t2.setX(xn2);
    t2.setY(yn2);
    shape->setStart(t1);
    shape->setEnd(t2);
}

void PaintWidget::SutherlandHodgman()
{
    float xmin = qMin(wins.x(),wine.x());
    float xmax = qMax(wins.x(),wine.x());
    float ymin = qMin(wins.y(),wine.y());
    float ymax = qMax(wins.y(),wine.y());

    QList<QPoint> list;
    for(int i = 0; i < 4;i ++)
    {
        int len = shape->pointList.length();
        if(i==0)
        {
            //xmin边界
            for(int j = 0; j < len; j ++)
            {
                QPoint t = shape->pointList[j];
                QPoint p = shape->pointList[(j-1+len)%len];
                if(p.x()<xmin && t.x()>=xmin)
                {
                    float ty = ((t.y()-p.y())*1.0) / (1.0*(t.x()-p.x())) * (1.0*(xmin-p.x())) + 1.0*p.y();
                    QPoint temp;
                    temp.setX(xmin);
                    temp.setY(ty);
                    list.append(temp);
                    list.append(t);
                }
                else if(t.x()>=xmin && p.x()>=xmin)
                {
                    list.append(t);
                }
                else if(p.x()>=xmin && t.x()<xmin)
                {
                    float ty = ((t.y()-p.y())*1.0) / (1.0*(t.x()-p.x())) * (1.0*(xmin-p.x())) + 1.0*p.y();
                    QPoint temp;
                    temp.setX(xmin);
                    temp.setY(ty);
                    list.append(temp);
                }
            }
        }
        else if(i==1)
        {
            //xmax边界
            for(int j = 0; j < len; j ++)
            {
                QPoint t = shape->pointList[j];
                QPoint p = shape->pointList[(j-1+len)%len];
                if(p.x()>xmax && t.x()<=xmax)
                {
                    float ty = ((t.y()-p.y())*1.0) / (1.0*(t.x()-p.x())) * (1.0*(xmax-p.x())) + 1.0*p.y();
                    QPoint temp;
                    temp.setX(xmax);
                    temp.setY(ty);
                    list.append(temp);
                    list.append(t);
                }
                else if(t.x()<=xmax && p.x()<=xmax)
                {
                    list.append(t);
                }
                else if(p.x()<=xmax && t.x()>xmax)
                {
                    float ty = ((t.y()-p.y())*1.0) / (1.0*(t.x()-p.x())) * (1.0*(xmax-p.x())) + 1.0*p.y();
                    QPoint temp;
                    temp.setX(xmax);
                    temp.setY(ty);
                    list.append(temp);
                }
            }
        }
        else if(i==2)
        {
            //ymin边界
            for(int j = 0; j < len; j ++)
            {
                QPoint t = shape->pointList[j];
                QPoint p = shape->pointList[(j-1+len)%len];
                if(p.y()<ymin && t.y()>=ymin)
                {
                    float tx;
                    if(p.x()==t.x())
                        tx = p.x();
                    else
                        tx = ((t.x()-p.x())*1.0) / (1.0*(t.y()-p.y())) * (1.0*(ymin-p.y())) + 1.0*p.x();

                    QPoint temp;
                    temp.setX(tx);
                    temp.setY(ymin);
                    list.append(temp);
                    list.append(t);
                }
                else if(t.y()>=ymin && p.y()>=ymin)
                {
                    list.append(t);
                }
                else if(p.y()>=ymin && t.y()<ymin)
                {
                    float tx;
                    if(p.x()==t.x())
                        tx = p.x();
                    else
                        tx = ((t.x()-p.x())*1.0) / (1.0*(t.y()-p.y())) * (1.0*(ymin-p.y())) + 1.0*p.x();

                    QPoint temp;
                    temp.setX(tx);
                    temp.setY(ymin);
                    list.append(temp);
                    list.append(temp);
                }
            }
        }
        else if(i==3)
        {
            //ymax边界
            for(int j = 0; j < len; j ++)
            {
                QPoint t = shape->pointList[j];
                QPoint p = shape->pointList[(j-1+len)%len];
                if(p.y()>ymax && t.y()<=ymax)
                {
                    float tx;
                    if(p.x()==t.x())
                        tx = p.x();
                    else
                        tx = ((t.x()-p.x())*1.0) / (1.0*(t.y()-p.y())) * (1.0*(ymax-p.y())) + 1.0*p.x();

                    QPoint temp;
                    temp.setX(tx);
                    temp.setY(ymax);
                    list.append(temp);
                    list.append(t);
                }
                else if(t.y()<=ymax && p.y()<=ymax)
                {
                    list.append(t);
                }
                else if(p.y()<=ymax && t.y()>ymax)
                {
                    float tx;
                    if(p.x()==t.x())
                        tx = p.x();
                    else
                        tx = ((t.x()-p.x())*1.0) / (1.0*(t.y()-p.y())) * (1.0*(ymax-p.y())) + 1.0*p.x();

                    QPoint temp;
                    temp.setX(tx);
                    temp.setY(ymax);
                    list.append(temp);
                }
            }
        }
        shape->pointList.clear();
        foreach(QPoint p, list)
        {
            QPoint t = p;
            shape->pointList.append(t);
        }
        list.clear();
    }
    shape->colorPoint.clear();
}

void PaintWidget::ShapeTrim()
{
    if(currShapeCode==Shape::Line)
    {
        LiangBarsky();
    }
    else if(currShapeCode==Shape::Polygon)
    {
        SutherlandHodgman();
    }
}

void PaintWidget::clearShapes()
{
    shapeList.clear();
    shape = NULL;
    painted = true;
    fill = false;
    solid = false;
    selectPoint = false;
    selectStart = false;
    selectEnd = false;
    pixNull = true;
    update();
}

void PaintWidget::setCurrentShape(Shape::Code s)
{
    if(s!=currShapeCode)
    {
        currShapeCode = s;
        solid = false;
    }
}

void PaintWidget::importPic()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        return;
    }
    pixmap.load(path);
    pixNull = false;
    update();
}

void PaintWidget::savePic()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Image"),".",tr("Images (*.png *.jpg *.bmp)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("No path selected."));
    }

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    pixmap.save(path);
}

void PaintWidget::importShape()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),".",tr("Text (*.json)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("No path selected."));
        return;
    }
    QFile File(path);
    File.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray byteArray = (QByteArray)File.readAll();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = document.object();
        QJsonValue value = object.value("type");
        int type = value.toVariant().toInt();
        switch(type)
        {
        case Shape::Line:
        {
            currShapeCode = Shape::Line;
            shape = new Line;
            solid = true;
            int startx = object.value("startx").toVariant().toInt();
            int starty = object.value("starty").toVariant().toInt();
            int endx = object.value("endx").toVariant().toInt();
            int endy = object.value("endy").toVariant().toInt();
            QPoint s,e;
            s.setX(startx);
            s.setY(starty);
            e.setX(endx);
            e.setY(endy);
            shape->setStart(s);
            shape->setEnd(e);
            shapeList<<shape;
            break;
        }
        case Shape::Circle:
        {
            currShapeCode = Shape::Circle;
            shape = new Circle;
            solid = true;
            int startx = object.value("startx").toVariant().toInt();
            int starty = object.value("starty").toVariant().toInt();
            int endx = object.value("endx").toVariant().toInt();
            int endy = object.value("endy").toVariant().toInt();
            QPoint s,e;
            s.setX(startx);
            s.setY(starty);
            e.setX(endx);
            e.setY(endy);
            shape->setStart(s);
            shape->setEnd(e);
            shapeList<<shape;
            break;
        }
        case Shape::Ellipse:
        {
            currShapeCode = Shape::Ellipse;
            shape = new Ellipse;
            solid = true;
            int startx = object.value("startx").toVariant().toInt();
            int starty = object.value("starty").toVariant().toInt();
            int endx = object.value("endx").toVariant().toInt();
            int endy = object.value("endy").toVariant().toInt();
            QPoint s,e;
            s.setX(startx);
            s.setY(starty);
            e.setX(endx);
            e.setY(endy);
            shape->setStart(s);
            shape->setEnd(e);
            shapeList<<shape;
            break;
        }
        case Shape::Polygon:
        {
            currShapeCode = Shape::Polygon;
            //selectPoint = true;
            solid = true;
            shape = new Polygon;
            QJsonValue value = object.value("points");
            QJsonArray array = value.toArray();
            int nSize = array.size();
            for (int i = 0; i < nSize; i++)
            {
                QPoint p;
                if(i%2==0)
                    p.setX(array.at(i).toInt());
                else
                    p.setY(array.at(i).toInt());

                i=i+1;
                if(i%2==0)
                    p.setX(array.at(i).toInt());
                else
                    p.setY(array.at(i).toInt());
                shape->pointList.append(p);
            }
            fill = false;
            shapeList<<shape;
            break;
        }
        case Shape::Bezier:
        {
            currShapeCode = Shape::Bezier;
            //selectPoint = true;
            solid = true;
            shape = new Bezier;
            QJsonValue value = object.value("points");
            QJsonArray array = value.toArray();
            int nSize = array.size();
            for (int i = 0; i < nSize; i++)
            {
                QPoint p;
                if(i%2==0)
                    p.setX(array.at(i).toInt());
                else
                    p.setY(array.at(i).toInt());

                i=i+1;
                if(i%2==0)
                    p.setX(array.at(i).toInt());
                else
                    p.setY(array.at(i).toInt());
                shape->pointList.append(p);
            }
            fill = false;
            shapeList<<shape;
            break;
        }
        case Shape::BSpline:
        {
            currShapeCode = Shape::BSpline;
            //selectPoint = true;
            solid = true;
            shape = new BSpline;
            QJsonValue value = object.value("points");
            QJsonArray array = value.toArray();
            int nSize = array.size();
            for (int i = 0; i < nSize; i++)
            {
                QPoint p;
                if(i%2==0)
                    p.setX(array.at(i).toInt());
                else
                    p.setY(array.at(i).toInt());

                i=i+1;
                if(i%2==0)
                    p.setX(array.at(i).toInt());
                else
                    p.setY(array.at(i).toInt());
                shape->pointList.append(p);
            }
            fill = false;
            shapeList<<shape;
            break;
        }
        }
    }
    update();
}

void PaintWidget::saveShape()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Json"),".",tr("Text (*.json)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("No path selected."));
    }

    switch(currShapeCode)
    {
    case Shape::Line: case Shape::Circle: case Shape::Ellipse:
    {
        QJsonObject json;
        json.insert("type", currShapeCode);
        json.insert("startx", shape->startPoint().x());
        json.insert("starty", shape->startPoint().y());
        json.insert("endx", shape->endPoint().x());
        json.insert("endy", shape->endPoint().y());

        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);

        QFile File(path);
        File.open(QIODevice::WriteOnly | QIODevice::Text);

        File.write(byteArray);
        break;
    }
    case Shape::Polygon: case Shape::Bezier: case Shape::BSpline:
    {
        QJsonArray array;
        foreach(QPoint p, shape->pointList)
        {
            array.append(p.x());
            array.append(p.y());
        }
        QJsonObject json;
        json.insert("type", currShapeCode);
        json.insert("points", QJsonValue(array));

        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);

        QFile File(path);
        File.open(QIODevice::WriteOnly | QIODevice::Text);

        File.write(byteArray);
        break;
    }
    }
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.drawRect(0,0,size().width(),size().height());

    if(!pixNull)
    {
        painter.drawPixmap(QPoint(0,0), pixmap);
        return;
    }

    if(trimWin)
    {
        painter.setPen(Qt::yellow);
        painter.drawRect(wins.x(),wins.y(),wine.x()-wins.x(),wine.y()-wins.y());
    }
    foreach(Shape * shape, shapeList)
    {
        shape->paint(painter);
    }
    if(shape)
    {
        shape->paint(painter);
    }
    if(shape && fill && painted && currShapeCode==Shape::Polygon)
    {
        painter.setPen(Qt::cyan);
        shape->fillShape(painter);
        fill=false;
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(trimWin == true)
    {
        wins=event->pos();
    }
    else if(event->button()==Qt::LeftButton)
    {
        if((currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline || currShapeCode==Shape::Polygon) && solid)
        {
            QPoint p = event->pos();
            for(int i = 0; i < shape->pointList.length(); i++)
            {
                if(abs(shape->pointList[i].x()-p.x())<=4 && abs(shape->pointList[i].y()-p.y())<=4)
                {
                    pointIndex = i;
                    selectPoint=true;
                    return;
                }
            }
        }
        else if((currShapeCode==Shape::Line || currShapeCode==Shape::Shape::Circle || currShapeCode==Shape::Shape::Ellipse) && solid)
        {
            QPoint p = event->pos();
            if(abs(shape->startPoint().x()-p.x())<=4 && abs(shape->startPoint().y()-p.y())<=4)
            {
                selectStart=true;
                return;
            }
            else if(abs(shape->endPoint().x()-p.x())<=4 && abs(shape->endPoint().y()-p.y())<=4)
            {
                selectEnd = true;
                return;
            }
        }
        switch(currShapeCode)
        {
            case Shape::Line:
            {
            solid = true;
                shape=new Line;
                break;
            }
            case Shape::Circle:
            {
            solid = true;
                shape=new Circle;
                break;
            }
            case Shape::Ellipse:
            {
            solid = true;
                shape=new Ellipse;
                break;
            }
            case Shape::Polygon:
            {
            solid = true;
                if(painted)
                {
                    painted = false;
                    shape=new Polygon;
                }
                painted = false;

                break;
            }
        case Shape::Bezier:
        {
        solid = true;
            if(painted)
            {
                painted = false;
                shape=new Bezier;
            }
            painted = false;

            break;
        }
        case Shape::BSpline:
        {
        solid = true;
            if(painted)
            {
                painted = false;
                shape=new BSpline;
            }
            painted = false;

            break;
        }
        }
        if(shape!=NULL && currShapeCode!=Shape::Polygon && currShapeCode!=Shape::Bezier && currShapeCode!=Shape::BSpline)
        {
            shape->setStart(event->pos());
        }
        else if(shape !=NULL && currShapeCode==Shape::Polygon)
        {
            shape->appendPoint(event->pos());
        }
        else if(shape != NULL && (currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline))
        {
            shape->appendPoint(event->pos());
        }
    }
    else if(event->button()==Qt::RightButton)
    {
        if(shape && (currShapeCode==Shape::Polygon || currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline))
        {
            fill = false;
            shapeList<<shape;
            update();
            painted = true;
        }
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(trimWin == true)
    {
        wine=event->pos();
        update();
    }
    else if(selectPoint && (currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline || currShapeCode==Shape::Polygon))
    {
        shape->pointList.removeAt(pointIndex);
        shape->pointList.insert(pointIndex, event->pos());
        shape->colorPoint.clear();
        update();
    }
    else if(selectStart && (currShapeCode==Shape::Line || currShapeCode==Shape::Shape::Circle || currShapeCode==Shape::Shape::Ellipse))
    {
        shape->setStart(event->pos());
        update();
    }
    else if(selectEnd && (currShapeCode==Shape::Line || currShapeCode==Shape::Shape::Circle || currShapeCode==Shape::Shape::Ellipse))
    {
        shape->setEnd(event->pos());
        update();
    }
    else if(currShapeCode!=Shape::Polygon && currShapeCode!=Shape::Bezier && currShapeCode!=Shape::BSpline)
    {
        shape->setEnd(event->pos());
        update();
    }
}


void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(trimWin)
    {
        wine=event->pos();
        update();
        if(solid)
            ShapeTrim();
        trimWin = false;
    }
    else if(event->button()==Qt::LeftButton)
    {
        if(shape && (currShapeCode==Shape::Bezier || currShapeCode==Shape::BSpline || currShapeCode==Shape::Polygon) && selectPoint)
        {
            selectPoint = false;
        }
        else if(shape && (currShapeCode==Shape::Line || currShapeCode==Shape::Shape::Circle || currShapeCode==Shape::Shape::Ellipse) && selectStart)
        {
            selectStart = false;
        }
        else if(shape && (currShapeCode==Shape::Line || currShapeCode==Shape::Shape::Circle || currShapeCode==Shape::Shape::Ellipse) && selectEnd)
        {
            selectEnd = false;
        }
        else if(shape && currShapeCode!=Shape::Polygon && currShapeCode!=Shape::Bezier && currShapeCode!=Shape::BSpline)
        {
            shape->setEnd(event->pos());
            shapeList<<shape;
            update();
        }
    }
}
