#ifndef CLOSECURVE_H
#define CLOSECURVE_H

#include "polygon.h"
class DLL CloseCurve : public Polygon
{
public:
    CloseCurve();
    QString name();
    virtual shared_ptr<GeneralShape >  copyPaste();

    //QString virtual qStringFromThis();

    //void draw(QPainter &painter,qreal zoomRatio);

    bool virtual isEmpty();
    void virtual mousePress(QPointF p);
    void virtual mouseMove(QPointF p);
    void virtual mouseRelease(QPointF p);
};

#endif // CLOSECURVE_H
