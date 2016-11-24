#ifndef CURVE_H
#define CURVE_H
 
#include "polyline.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Curve : public Polyline
{
public:
    Curve();
    QString name();
    virtual Curve *  copyPaste();
    //QString virtual qStringFromThis();

 
protected:
 
private:
};
 
#endif // CURVE_H
