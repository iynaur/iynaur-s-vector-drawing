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
//QString Curve:: qStringFromThis(){
//    //qDebug()<<"name="<<metaObject()->className();
//    return "Curve"+qStringFromPoints();
//}
Curve *  Curve::copyPaste(){
   return new Curve(*this);
}
