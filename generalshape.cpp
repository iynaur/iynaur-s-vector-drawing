#include "generalshape.h"
#include<QPainter>
GeneralShape::GeneralShape()


{
    points=QVector<QPointF>(0);
}
GeneralShape::~GeneralShape(){

}

void GeneralShape:: fromline(QString line){
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


QPointF GeneralShape:: centralPoint(){
    return QPointF((minx+maxx)/2,(miny+maxy)/2);
}

void GeneralShape:: zoom(qreal zoomratio){

    updateRange();
}
void GeneralShape:: zoom(qreal zx,qreal zy){

    updateRange();
}


bool GeneralShape:: inRange(QPointF p0,QPointF p1){

        double left=min(p0.x(),p1.x());
        double right=max(p0.x(),p1.x());
        double top=min(p0.y(),p1.y());
        double bottom=max(p0.y(),p1.y());
        if (this->left>left && this->right<right && this->top>top && this->bottom<bottom) return true;
        else return false;

}
void GeneralShape:: updateBand(){
    QPointF p0=QPointF((maxx+minx)/2,(maxy+miny)/2);
//        QPointF* p= new QPointF[4];
    QPointF p[4];
    p[0]=QPointF(minx,miny)-p0;
    p[1]=QPointF(maxx,miny)-p0;
    p[2]=QPointF(minx,maxy)-p0;
    p[3]=QPointF(maxx,maxy)-p0;
    for (int i=0;i<4;i++){
        p[i]=QPointF(p[i].x()*getsx(),p[i].y()*getsy());
        p[i]=rotated(p[i],rotationangle()/180*M_PI);
        p[i]=p[i]+p0;
    }
    top=bottom=p[0].y();
    left=right=p[0].x();
    for (int i=1;i<4;i++){
        top=min(top,p[i].y());
        bottom=max(bottom,p[i].y());
        left=min(left,p[i].x());
        right=max(right,p[i].x());
    }
    //qDebug()<<top<<bottom<<left<<right;


}

//void GeneralShape:: drag(QPointF point)
//{

//    updateRange();
//}
bool GeneralShape:: isEmpty(){
    return false;
}
qreal GeneralShape::rotationangle(){
    return Rotationangle;
}
void GeneralShape::setRotationangle(qreal angle){
    Rotationangle=angle;
    updateBand();
}

void GeneralShape:: setsx(double x){
    sx=x;
    updateBand();
}
void GeneralShape:: setsy(double y){
    sy=y;
    updateBand();
}
double GeneralShape:: getsy(){
    return sy;
}
double GeneralShape:: getsx(){
    return sx;
}
void GeneralShape:: drawClosure(QPainter &painter, qreal zoomRatio){
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    //QBrush defaultbrush=QBrush(QColor(ARGB 1, 0, 0, 0) , Qt::NoBrush );
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
QPointF GeneralShape:: rotationHandlePoint(){
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
QPointF GeneralShape:: scaleHandlePoint(){
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
