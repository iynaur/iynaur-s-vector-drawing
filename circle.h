#ifndef CIRCLE
#define CIRCLE
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class Circle : public AbstractShape
{
public:
    Circle();
    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Circle"+qStringFromPoints();
    }
    virtual Circle *  copyPaste(){
        Circle* tmp=new Circle;
        tmp->points=points;
        tmp->name=name;
        tmp->pen=pen;
        tmp->Rotationangle=Rotationangle;
        tmp->sx=sx;
        tmp->sy=sy;
        tmp->brush=brush;
        return tmp;
    }
    //void addPoint(QPointF point);
    void drag(QPointF point);
    void draw(QPainter &painter,qreal zoomRatio);
    //void removeLastPoint();
    double   minDistance(QPointF point);
    void updateRange();
    bool inRange(QPoint p0,QPoint p1);
    void  drawClosure(QPainter &painter,qreal zoomRatio);
    QPointF virtual rotationHandlePoint(){ return points.at(0);}
    QPointF  scaleHandlePoint();
    double virtual getsy(){
        return sx;
    }

protected:

private:

};
#endif // CIRCLE

