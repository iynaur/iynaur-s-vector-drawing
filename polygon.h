#ifndef PG_H
#define PG_H
 
#include "polyline.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Polygon : public Polyline
{
public:
    Polygon();
    QString name();
    virtual Polygon *  copyPaste();
    //QString virtual qStringFromThis();
    double  virtual minDistance(QPointF point);

    void draw(QPainter &painter,qreal zoomRatio);

    bool virtual isEmpty();


 
protected:
 

};
 
#endif //BK_H
