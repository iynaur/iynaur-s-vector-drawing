#include "abstractshape.h"
 
AbstractShape::AbstractShape()
{
    //setBackgroundRole(QPalette::Base);
    minx=miny=maxx=maxy=0;
    Rotationangle=0;
    sx=1;
    sy=1;
}
 
void AbstractShape::setPen(const QPen &pen)
{
    this->pen = pen;
}
 
void AbstractShape::setBrush(const QBrush &brush)
{
    this->brush = brush;
}

QString AbstractShape::qStringFromPoints(){
    QString tmp="";
            for(int i=0;i<points.size();i++){
                tmp=tmp+"("+QString::number(points.at(i).x())+","+QString::number(points.at(i).y())+")";
            }
            return tmp;
}

//    QString   qStringFromThis()=0;

//void   AbstractShape::draw(QPainter &painter,qreal zoomRatio)=0;//需要具体图形子类实现
void  AbstractShape::addPoint(QPointF point) {

        points.append(point);
        updateRange();

}
     //为了画任意曲线特意留的一个口子
void  AbstractShape::removeLastPoint(){
    points.removeLast();
}
//double   AbstractShape::minDistance(QPointF point)=0;
//void   AbstractShape::drawClosure(QPainter &painter,qreal zoomRatio)=0;
//QPointF   AbstractShape::rotationHandlePoint()=0;
QPointF   AbstractShape::scaleHandlePoint(){return points.at(0);}
void  AbstractShape::zoom(qreal zoomratio){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()*zoomratio);
        points[i].setY(points.at(i).y()*zoomratio);

    }
    updateRange();
}
void  AbstractShape::zoom(qreal zx,qreal zy){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()*zx);
        points[i].setY(points.at(i).y()*zy);

    }
    updateRange();
}
void  AbstractShape::fromline(QString line){
    points.clear();
    int a,b,c;
    a=b=c=0;
    while(line.indexOf("(",a)>=0){
        a=line.indexOf("(",a);
        c=line.indexOf(",",a);
        b=line.indexOf(")",a);
        double x=line.mid(a+1,c-a-1).toDouble();
        double y=line.mid(c+1,b-c-1).toDouble();
        addPoint(QPointF(x,y));
        a=b;
    }
}

bool  AbstractShape::inRange(QPointF p0,QPointF p1){

        double left=min(p0.x(),p1.x());
        double right=max(p0.x(),p1.x());
        double top=min(p0.y(),p1.y());
        double bottom=max(p0.y(),p1.y());
        if (minx>left && maxx<right && miny>top && maxy<bottom) return true;
        else return false;

}

void  AbstractShape::drag(QPointF point)
{
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
    updateRange();
}
bool  AbstractShape::isEmpty(){
    return false;
}
