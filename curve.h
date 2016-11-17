#ifndef CURVE_H
#define CURVE_H
 
#include "poly.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Curve : public Poly
{
public:
    Curve();

    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Curve"+qStringFromPoints();
    }
 
    //void draw(QPainter &painter,qreal zoomRatio);
    //void virtual drawClosure(QPainter &painter, qreal zoomRatio);
//    QPointF rotationHandlePoint();

 
protected:
 
private:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
};
 
#endif // CURVE_H
