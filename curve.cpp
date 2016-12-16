#include "curve.h"
#include <algorithm>
using namespace std;
 
Curve::Curve()
{
    //name="Curve";
}
 
 
QString Curve::name(){
    return "Curve";
}

shared_ptr<GeneralShape> Curve::copyPaste(){
    shared_ptr<Curve> tmp=shared_ptr<Curve>(new Curve);
    tmp->points=points;
    //tmp->name=name;
    tmp->pen=pen;
    tmp->Rotationangle=Rotationangle;
    tmp->sx=sx;
    tmp->sy=sy;
    tmp->brush=brush;
    tmp->updateRange();
    return static_pointer_cast<GeneralShape>(tmp);
}
void Curve::mousePress(QPointF p){
    addPoint(p);
}

void Curve::mouseMove(QPointF p){
    addPoint(p);
}

void Curve::mouseRelease(QPointF p){

}
