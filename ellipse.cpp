#include "ellipse.h"
#include<cmath>
//#define PI 3.14159265
Ellipse::Ellipse()
{
    name="Ellipse";
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

void Ellipse::drawClosure(QPainter &painter, qreal zoomRatio){
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);
    painter.setBrush( QBrush( Qt::NoBrush ));
    double left=min(points.at(0).x(),points.at(1).x());
    double right=max(points.at(0).x(),points.at(1).x());
    double top=min(points.at(0).y(),points.at(1).y());
    double bottom=max(points.at(0).y(),points.at(1).y());
    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );
    painter.drawRect((left-right)/2*zoomRatio*sx,(top-bottom)/2*zoomRatio*sy,
                     (right-left)*zoomRatio*sx,(bottom-top)*zoomRatio*sy);
    painter.drawLine(QPointF(0,(top-bottom)/2*zoomRatio*sy) ,
                     QPointF(0,(top-bottom)/2*zoomRatio*sy-sy/abs(sy)*lenthOfRotationHandleLine));
    painter.setPen(QPen(Qt::black,3));
    painter.drawPoint(-(left-right)/2*zoomRatio*sx,-(top-bottom)/2*zoomRatio*sy);
    painter.setPen(pen);
    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);
}

QPointF Ellipse::rotationHandlePoint(){
    double left=min(points.at(0).x(),points.at(1).x());
    double right=max(points.at(0).x(),points.at(1).x());
    double top=min(points.at(0).y(),points.at(1).y());
    double bottom=max(points.at(0).y(),points.at(1).y());
    //double x=0;
    double y=(top-bottom)/2*sy;
    double x1=-y*sin(Rotationangle/180*M_PI);
    double y1=y*cos(Rotationangle/180*M_PI);
    return QPointF(x1+(left+right)/2,y1+(top+bottom)/2);

}
QPointF Ellipse::scaleHandlePoint(){
    double left=min(points.at(0).x(),points.at(1).x());
    double right=max(points.at(0).x(),points.at(1).x());
    double top=min(points.at(0).y(),points.at(1).y());
    double bottom=max(points.at(0).y(),points.at(1).y());
    double x=(left-right)/2*sx;
    double y=(top-bottom)/2*sy;
    double sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)-y*sin(sita);
    double y1=x*sin(sita)+y*cos(sita);
    return QPointF(-x1+(left+right)/2,-y1+(top+bottom)/2);
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
//    if (min(a,b)<3){

//        double min1=min(abs(x-a) , abs(x+a));
//        double min2=min(abs(y-b) , abs(y+b));
//        if ((x-a)*(x+a) >0){
//            if ((y-b)*(y+b)>0)
//                return sqrt(min1*min1+min2*min2);
//            else
//                return min1;

//        }else{
//            if ((y-b)*(y+b)>0)
//                return min2;
//            else
//                return 0;
//        }
//    }else{
    //qDebug()<<abs( sqrt( (x/a)*(x/a) + (y/b)*(y/b) ) - 1 )*(a+b)/2  ;
    return  max(( sqrt( (x/a)*(x/a) + (y/b)*(y/b) ) - 1 )*(a+b)/2 , 0.0) ;
//    }
}
void Ellipse::drag(QPoint point){
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
bool Ellipse::inRange(QPoint p0,QPoint p1){
    int left=min(p0.x(),p1.x());
    int right=max(p0.x(),p1.x());
    int top=min(p0.y(),p1.y());
    int bottom=max(p0.y(),p1.y());
    if (minx>left && maxx<right && miny>top && maxy<bottom) return true;
    else return false;
}


