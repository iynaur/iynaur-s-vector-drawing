#ifndef ELLIPSE
#define ELLIPSE
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class Ellipse : public AbstractShape
{
public:
    Ellipse();
    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Ellipse"+qStringFromPoints();
    }
    virtual Ellipse *  copyPaste(){
        Ellipse* tmp=new Ellipse;
        tmp->points=points;
        tmp->name=name;
        tmp->pen=pen;
        tmp->Rotationangle=Rotationangle;
        tmp->sx=sx;
        tmp->sy=sy;
        tmp->brush=brush;
        return tmp;
    }
    bool virtual isEmpty(){
        if (points.at(0)==points.at(1)) return true;
        else return false;
    }
    //void addPoint(QPoint point);
    void drag(QPoint point);
    void draw(QPainter &painter,qreal zoomRatio);
    void removeLastPoint();
    double   minDistance(QPointF point);
    void updateRange();
    bool inRange(QPoint p0,QPoint p1);
    void virtual drawClosure(QPainter &painter, qreal zoomRatio);
    QPointF virtual rotationHandlePoint();
    QPointF scaleHandlePoint();

protected:

private:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
};
#endif // ELLIPSE

