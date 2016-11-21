#include "circle.h"
#include<cmath>

Circle::Circle()
{
    //name="Circle";
}

QString Circle::name()
{ return "Circle";}

void Circle::draw(QPainter &painter, qreal zoomRatio)
{
    painter.setPen(pen);
    painter.setBrush(brush);
    double r=sqrt(pow((points.at(0).x()-points.at(1).x()),2)+pow((points.at(0).y()-points.at(1).y()),2));
    r=r*sx;


    painter.drawEllipse((points.at(0).x()-r)*zoomRatio,(points.at(0).y()-r)*zoomRatio,2*r*zoomRatio,2*r*zoomRatio);
}

void  Circle::drawClosure(QPainter &painter,qreal zoomRatio){
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush( QBrush( Qt::NoBrush ));
    double r=sqrt(pow((points.at(0).x()-points.at(1).x()),2)+pow((points.at(0).y()-points.at(1).y()),2));
    r=r*sx;
    painter.drawRect((points.at(0).x()-r)*zoomRatio,(points.at(0).y()-r)*zoomRatio,2*r*zoomRatio,2*r*zoomRatio);
    painter.setPen(QPen(Qt::black,3));
    painter.drawPoint((points.at(0).x()+r)*zoomRatio,(points.at(0).y()+r)*zoomRatio);
    painter.setPen(pen);
}
QPointF Circle::scaleHandlePoint(){
    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;
    double x=(left-right)/2*sx;
    double y=(top-bottom)/2*sx;
    double sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)-y*sin(sita);
    double y1=x*sin(sita)+y*cos(sita);
    return QPointF(-x1+(left+right)/2,-y1+(top+bottom)/2);
}
//void Circle::removeLastPoint(){
//    points.removeLast();
//}
double Circle::minDistance(QPointF point){
    double r=sqrt(pow((points.at(0).x()-points.at(1).x()),2)+pow((points.at(0).y()-points.at(1).y()),2));
   double d=sqrt(pow((points.at(0).x()-point.x()),2)+pow((points.at(0).y()-point.y()),2));

    return max(d-r*abs(sx),0.0);
}
void Circle::drag(QPointF point){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
//            foreach(QPointF p,points){
//                p += point;
//            }
    updateRange();
    //qDebug()<<"moved"<<(point);
}
void Circle::updateRange(){
    double r;
    if (points.size()<2){
        r=0;
    }
    else{
        r=sqrt(pow((points.at(0).x()-points.at(1).x()),2)+pow((points.at(0).y()-points.at(1).y()),2));
    }
    minx=points.at(0).x()-r;
    miny=points.at(0).y()-r;
    maxx=points.at(0).x()+r;
    maxy=points.at(0).y()+r;
    updateBand();
    //qDebug()<<"maxx="<<maxx;
}
bool Circle::inRange(QPoint p0,QPoint p1){
    int left=min(p0.x(),p1.x());
    int right=max(p0.x(),p1.x());
    int top=min(p0.y(),p1.y());
    int bottom=max(p0.y(),p1.y());
    if (minx>left && maxx<right && miny>top && maxy<bottom) return true;
    else return false;
}
//QString Circle:: qStringFromThis(){
//    //qDebug()<<"name="<<metaObject()->className();
//    return "Circle"+qStringFromPoints();
//}
 Circle *  Circle::copyPaste(){
    Circle* tmp=new Circle;
    tmp->points=points;
    //tmp->name=name;
    tmp->pen=pen;
    tmp->Rotationangle=Rotationangle;
    tmp->sx=sx;
    tmp->sy=sy;
    tmp->brush=brush;
    return tmp;
}

QPointF Circle:: rotationHandlePoint(){ return points.at(0);}
double Circle:: getsy(){
    return sx;
}
bool  Circle::isEmpty(){
    if (points.at(0)==points.at(1))
        return true;
    else
        return false;
}
