#ifndef CURVE_H
#define CURVE_H
 
#include "polyline.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class DLL Curve : public Polyline
{
public:
    Curve();
    QString name();
    virtual shared_ptr<GeneralShape>  copyPaste();
    //QString virtual qStringFromThis();
    void virtual mousePress(QPointF p);
    void virtual mouseMove(QPointF p);
    void virtual mouseRelease(QPointF p);


 
protected:
 
private:
};
 
#endif // CURVE_H
