#ifndef GS_H
#define GS_H
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#include<cmath>
#include<mymath.h>
using namespace std;
const double MAX=10000;
const double  lenthOfRotationHandleLine=20;

class GeneralShape : public QWidget
{
    Q_OBJECT
 
public:
    qreal minx,miny,maxx,maxy;
    qreal top,bottom,left,right;
    QString name;
 
    explicit GeneralShape(QWidget *parent = 0);
    virtual GeneralShape *  copyPaste()=0;


    QString virtual qStringFromPoints()=0;
    void virtual fromline(QString line){
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

    void virtual draw(QPainter &painter,qreal zoomRatio)=0;//需要具体图形子类实现

         //为了画任意曲线特意留的一个口子

    double virtual minDistance(QPointF point)=0;
    void virtual drawClosure(QPainter &painter,qreal zoomRatio)=0;
    QPointF virtual rotationHandlePoint()=0;
    QPointF virtual scaleHandlePoint()=0;
    QPointF virtual centralPoint(){
        return QPointF((minx+maxx)/2,(miny+maxy)/2);
    }

    void virtual zoom(qreal zoomratio){

        updateRange();
    }
    void virtual zoom(qreal zx,qreal zy){

        updateRange();
    }


    bool  inRange(QPointF p0,QPointF p1){

            double left=min(p0.x(),p1.x());
            double right=max(p0.x(),p1.x());
            double top=min(p0.y(),p1.y());
            double bottom=max(p0.y(),p1.y());
            if (this->left>left && this->right<right && this->top>top && this->bottom<bottom) return true;
            else return false;

    }
    void virtual updateRange() =0;
    void virtual updateBand(){
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

    void virtual drag(QPointF point)
    {

        updateRange();
    }
    bool virtual isEmpty(){
        return false;
    }
    qreal rotationangle(){
        return Rotationangle;
    }
    void setRotationangle(qreal angle){
        Rotationangle=angle;
        updateBand();
    }
    void virtual addPoint(QPointF point)=0;
    void virtual removeLastPoint()=0;
    void virtual setPen(const QPen &pen)=0;
    void virtual setBrush(const QBrush &brush)=0;
    void virtual setsx(double x){
        sx=x;
        updateBand();
    }
    void virtual setsy(double y){
        sy=y;
        updateBand();
    }
    double virtual getsy(){
        return sy;
    }
    double virtual getsx(){
        return sx;
    }


protected:    
    QPen pen;

public:
    QVector<QPointF> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    qreal Rotationangle;
    qreal sx,sy;
    QBrush brush;

};


 
#endif // GS_H
