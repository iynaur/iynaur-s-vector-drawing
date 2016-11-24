#ifndef CLOSECURVE_H
#define CLOSECURVE_H

#include "Polygon.h"
class CloseCurve : public Polygon
{
public:
    CloseCurve();
    QString name();
    virtual CloseCurve *  copyPaste();

    //QString virtual qStringFromThis();

    //void draw(QPainter &painter,qreal zoomRatio);

    bool virtual isEmpty();
};

#endif // CLOSECURVE_H
