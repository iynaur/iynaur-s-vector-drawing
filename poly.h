#ifndef POLY_H
#define POLY_H
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
//#include "Polyline.h"
class Poly : public AbstractShape
{
public:
    Poly();
    void updateRange();
    void  drag(QPointF point);
//    void  removeLastPoint(){
//        points.removeLast();
//    }
    //QString virtual qStringFromThis();
    void copypoly(Poly* tmp);
    QString name();
    virtual Poly *  copyPaste();
    void virtual draw(QPainter &painter,qreal zoomRatio);
    double  virtual minDistance(QPointF point);
    bool virtual isEmpty();
    QPointF virtual rotationHandlePoint();
    QPointF virtual scaleHandlePoint();
    void virtual drawClosure(QPainter &painter, qreal zoomRatio);
};

#endif // POLY_H
