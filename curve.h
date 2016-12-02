#ifndef CURVE_H
#define CURVE_H
 
#include "polyline.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Curve : public Polyline
{
public:
    Curve();
    QString name();
    virtual shared_ptr<GeneralShape>  copyPaste();
    //QString virtual qStringFromThis();

 
protected:
 
private:
};
 
#endif // CURVE_H
