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
 shared_ptr<GeneralShape> CloseCurve::copyPaste(){
    //return new CloseCurve(*this);
    CloseCurve* tmp=new CloseCurve(*this);


    return static_pointer_cast<GeneralShape>(shared_ptr<CloseCurve>(tmp));

}
