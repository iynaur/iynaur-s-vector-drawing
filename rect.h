#ifndef RECT
#define RECT
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class Rect : public AbstractShape
{
public:
    Rect();
    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Rect"+qStringFromPoints();
    }
    virtual Rect *  copyPaste(){
        Rect* tmp=new Rect;
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
    void virtual draw(QPainter &painter,qreal zoomRatio);
    //void removeLastPoint();
    double virtual   minDistance(QPointF point);
    void  updateRange();
    bool inRange(QPointF p0, QPointF p1);
    void virtual drawClosure(QPainter &painter, qreal zoomRatio);
    QPointF virtual rotationHandlePoint();
    QPointF  scaleHandlePoint();

protected:


    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
};
#endif // RECT

