#ifndef CLOSECURVE_H
#define CLOSECURVE_H

#include "polygon.h"
class CloseCurve : public Polygon
{
public:
    CloseCurve();
    QString name();
    virtual shared_ptr<GeneralShape >  copyPaste();

    //QString virtual qStringFromThis();

    //void draw(QPainter &painter,qreal zoomRatio);

    bool virtual isEmpty();
};

#endif // CLOSECURVE_H
