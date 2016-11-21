#ifndef PL_H
#define PL_H
 
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Polyline : public AbstractShape
{
public:
    Polyline();
    QString name();
    //QString virtual qStringFromThis();
    void updateRange();
    void  drag(QPointF point);
//    void  removeLastPoint(){
//        points.removeLast();
//    }
    //QString virtual qStringFromThis();
    void copypolyline(Polyline* tmp);
    virtual Polyline *  copyPaste();
    void virtual draw(QPainter &painter,qreal zoomRatio);
    double  virtual minDistance(QPointF point);
    bool virtual isEmpty();
    QPointF virtual rotationHandlePoint();
    QPointF virtual scaleHandlePoint();
    void virtual drawClosure(QPainter &painter, qreal zoomRatio);



};
 
#endif //BK_H
