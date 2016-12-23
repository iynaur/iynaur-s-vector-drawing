#include "ellipse.h"
#include<cmath>
//#define PI 3.14159265
Ellipse::Ellipse()
{
    //name="Ellipse";
}
QString Ellipse::name(){
    return "Ellipse";
}

//void Ellipse::addPoint(QPoint point)
//{
//    this->points.append(point);
//    updateRange();
//}

void Ellipse::draw(QPainter &painter, qreal zoomRatio)
{
    painter.setPen(pen);
    painter.setBrush(brush);
    double left=min(points.at(0).x(),points.at(1).x());
    double right=max(points.at(0).x(),points.at(1).x());
    double top=min(points.at(0).y(),points.at(1).y());
    double bottom=max(points.at(0).y(),points.at(1).y());
    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );
    painter.drawEllipse((left-right)/2*zoomRatio*sx,(top-bottom)/2*zoomRatio*sy,(right-left)*zoomRatio*sx,(bottom-top)*zoomRatio*sy);
    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);
}





void Ellipse::removeLastPoint(){
    points.removeLast();
}
double Ellipse::minDistance(QPointF point){//need re-caculate

    double x0=(maxx+minx)/2;
    double y0=(maxy+miny)/2;
    double a=(maxx-minx)/2*abs(sx);
    double b=(maxy-miny)/2*abs(sy);
    double x=point.x()-x0;
    double y=point.y()-y0;
    //recaculate position in translated Coordinate System
    //todo

    qreal sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)+y*sin(sita);
    double y1=-x*sin(sita)+y*cos(sita);
    x=x1;y=y1;

    return  max(( sqrt( (x/a)*(x/a) + (y/b)*(y/b) ) - 1 )*(a+b)/2 , 0.0) ;

}
void Ellipse::drag(QPointF point){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
    updateRange();
    //qDebug()<<"moved"<<(point);
}
void Ellipse::updateRange(){
    if (points.size()<2) return;
    minx=min(points.at(0).x(),points.at(1).x());
    maxx=max(points.at(0).x(),points.at(1).x());
    miny=min(points.at(0).y(),points.at(1).y());
    maxy=max(points.at(0).y(),points.at(1).y());
    updateBand();

}



 shared_ptr<GeneralShape> Ellipse::copyPaste(){
     shared_ptr<Ellipse> tmp=shared_ptr<Ellipse>(new Ellipse);
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
 bool Ellipse:: isEmpty(){
     if (points.size()>1 && (points.at(0)-points.at(1)).x()*(points.at(0)-points.at(1)).y()!=0) return false;
     else return true;
 }
