#include "generalshape.h"
GeneralShape::GeneralShape(QWidget *parent) :
    QWidget(parent)

{

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
//QString virtual qStringFromThis()=0;


     //为了画任意曲线特意留的一个口子


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

void GeneralShape:: drag(QPointF point)
{

    updateRange();
}
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
