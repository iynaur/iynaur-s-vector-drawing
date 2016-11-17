#ifndef PL_H
#define PL_H
 
#include "Poly.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Polyline : public Poly
{
public:
    Polyline();
    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Polyline"+qStringFromPoints();
    }

    //void draw(QPainter &painter,qreal zoomRatio);


 
protected:
 
private:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    //QPoint curpoint;

};
 
#endif //BK_H
