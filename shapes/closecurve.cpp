#include "closecurve.h"

CloseCurve::CloseCurve()
{
    //name="CloseCurve";
}


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
 void CloseCurve::mousePress(QPointF p){
     appendPoint(p);
 }

 void CloseCurve::mouseMove(QPointF p){
     appendPoint(p);
 }

 void CloseCurve::mouseRelease(QPointF p){

 }
