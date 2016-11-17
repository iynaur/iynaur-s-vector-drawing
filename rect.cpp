#include "rect.h"
#include<cmath>

Rect::Rect()
{
    name="Rect";

}

//void Rect::addPoint(QPointF point)
//{
//    this->points.append(point);
//    updateRange();
//}

void Rect::draw(QPainter &painter, qreal zoomRatio)
{
    painter.setPen(pen);
    painter.setBrush(brush);
    //qDebug()<<brush;
    double left=min(points.at(0).x(),points.at(1).x());
    double right=max(points.at(0).x(),points.at(1).x());
    double top=min(points.at(0).y(),points.at(1).y());
    double bottom=max(points.at(0).y(),points.at(1).y());
    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );
    painter.drawRect((left-right)/2*zoomRatio*sx,(top-bottom)/2*zoomRatio*sy,(right-left)*zoomRatio*sx,(bottom-top)*zoomRatio*sy);
    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);


}


QPointF Rect::rotationHandlePoint(){
        double left=minx;
        double right=maxx;
        double top=miny;
        double bottom=maxy;
    //double x=0;
    double y=(top-bottom)/2*sy;
    double x1=-y*sin(Rotationangle/180*M_PI);
    double y1=y*cos(Rotationangle/180*M_PI);
    return QPointF(x1+(left+right)/2,y1+(top+bottom)/2);

}
QPointF Rect::scaleHandlePoint(){
        double left=minx;
        double right=maxx;
        double top=miny;
        double bottom=maxy;
    double x=(left-right)/2*sx;
    double y=(top-bottom)/2*sy;
    double sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)-y*sin(sita);
    double y1=x*sin(sita)+y*cos(sita);
    return QPointF(-x1+(left+right)/2,-y1+(top+bottom)/2);
}
void Rect::drawClosure(QPainter &painter, qreal zoomRatio){
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);
    painter.setBrush( QBrush( Qt::NoBrush ));
    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;
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

//void Rect::removeLastPoint(){
//    points.removeLast();
//}
double Rect::minDistance(QPointF point){
    double x0=(maxx+minx)/2;
    double y0=(maxy+miny)/2;
    double a=(maxx-minx)/2*sx;
    double b=(maxy-miny)/2*sy;
    double x=point.x()-x0;
    double y=point.y()-y0;
    qreal sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)+y*sin(sita);
    double y1=-x*sin(sita)+y*cos(sita);
    x=x1;y=y1;


    double min1=min(abs(x-a) , abs(x+a));
    double min2=min(abs(y-b) , abs(y+b));
    if ((x-a)*(x+a) >0){
        if ((y-b)*(y+b)>0)
            return sqrt(min1*min1+min2*min2);
        else
            return min1;

    }else{
        if ((y-b)*(y+b)>0)
            return min2;
        else
            return 0;
    }
}
void Rect::drag(QPointF point){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
    updateRange();
    //qDebug()<<"moved"<<(point);
}
void Rect::updateRange(){
    //qDebug()<<"Rect::updateRange()";
    if (points.size()<2) return;
    minx=min(points.at(0).x(),points.at(1).x());
    maxx=max(points.at(0).x(),points.at(1).x());
    miny=min(points.at(0).y(),points.at(1).y());
    maxy=max(points.at(0).y(),points.at(1).y());
    updateBand();
}
bool Rect::inRange(QPointF p0,QPointF p1){
    double left=min(p0.x(),p1.x());
    double right=max(p0.x(),p1.x());
    double top=min(p0.y(),p1.y());
    double bottom=max(p0.y(),p1.y());
    if (minx>left && maxx<right && miny>top && maxy<bottom) return true;
    else return false;
}

