#ifndef ABSTRACTSHAPE_H
#define ABSTRACTSHAPE_H
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#include<cmath>
#include<mymath.h>
#include<generalshape.h>
using namespace std;


class DLL AbstractShape : public GeneralShape
{
 
public:
    AbstractShape();
    //~AbstractShape();
    virtual  shared_ptr<GeneralShape>    copyPaste()=0;


    QString qStringFromPoints();

    void virtual draw(QPainter &painter,qreal zoomRatio)=0;//需要具体图形子类实现
    void  appendPoint(QPointF point) ;
         //为了画任意曲线特意留的一个口子
    void  removeLastPoint();
    double virtual minDistance(QPointF point)=0;
    //void virtual drawClosure(QPainter &painter,qreal zoomRatio);
    //QPointF virtual rotationHandlePoint();
    //QPointF virtual scaleHandlePoint();
    void  zoom(qreal zoomratio);
    void  zoom(qreal zx,qreal zy);
    void  fromline(QString line);

    //bool  inRange(QPointF p0,QPointF p1);
    void virtual updateRange() =0;
    void  drag(QPointF point);
    bool  isEmpty();


    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
 



};


 
#endif // ABSTRACTSHAPE_H
