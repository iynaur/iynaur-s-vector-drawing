#ifndef CLOSECURVE_H
#define CLOSECURVE_H

#include "Polygon.h"
class CloseCurve : public Polygon
{
public:
    CloseCurve();

    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "CloseCurve"+qStringFromPoints();
    }

    //void draw(QPainter &painter,qreal zoomRatio);

    bool virtual isEmpty(){
        if (points.size()<3) return true;
        else return false;
    }
};

#endif // CLOSECURVE_H
