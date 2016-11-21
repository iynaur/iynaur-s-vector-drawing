#include "closecurve.h"

CloseCurve::CloseCurve()
{
    //name="CloseCurve";
}
//QString CloseCurve:: qStringFromThis(){
//    //qDebug()<<"name="<<metaObject()->className();
//    return "CloseCurve"+qStringFromPoints();
//}

//void draw(QPainter &painter,qreal zoomRatio);

bool CloseCurve:: isEmpty(){
    if (points.size()<3) return true;
    else return false;
}
QString CloseCurve::name(){
    return "CloseCurve";
}
