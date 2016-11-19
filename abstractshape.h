#ifndef ABSTRACTSHAPE_H
#define ABSTRACTSHAPE_H
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#include<cmath>
#include<mymath.h>
#include<generalshape.h>
using namespace std;


class AbstractShape : public GeneralShape
{
 
public:
    AbstractShape();
//    qreal minx,miny,maxx,maxy;
//    QString name;
 
//    explicit AbstractShape(QWidget *parent = 0);
    virtual AbstractShape *  copyPaste()=0;
//    virtual AbstractShape *  copyPaste(){
//        AbstractShape* tmp;
//        tmp->points=points;
//        tmp->name=name;
//        tmp->pen=pen;
//        tmp->Rotationangle=Rotationangle;
//        tmp->sx=sx;
//        tmp->sy=sy;
//        tmp->brush=brush;
//        return tmp;
//    }

    QString qStringFromPoints(){
        QString tmp="";
                for(int i=0;i<points.size();i++){
                    tmp=tmp+"("+QString::number(points.at(i).x())+","+QString::number(points.at(i).y())+")";
                }
                return tmp;
    }

//    QString virtual qStringFromThis()=0;

    void virtual draw(QPainter &painter,qreal zoomRatio)=0;//需要具体图形子类实现
    void  addPoint(QPointF point) {

            points.append(point);
            updateRange();

    }
         //为了画任意曲线特意留的一个口子
    void  removeLastPoint(){
        points.removeLast();
    }
    double virtual minDistance(QPointF point)=0;
    void virtual drawClosure(QPainter &painter,qreal zoomRatio)=0;
    QPointF virtual rotationHandlePoint()=0;
    QPointF virtual scaleHandlePoint(){return points.at(0);}
    void  zoom(qreal zoomratio){
        for (int i=0;i<points.size();i++){
            points[i].setX(points.at(i).x()*zoomratio);
            points[i].setY(points.at(i).y()*zoomratio);

        }
        updateRange();
    }
    void  zoom(qreal zx,qreal zy){
        for (int i=0;i<points.size();i++){
            points[i].setX(points.at(i).x()*zx);
            points[i].setY(points.at(i).y()*zy);

        }
        updateRange();
    }
    void  fromline(QString line){
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

    bool  inRange(QPointF p0,QPointF p1){

            double left=min(p0.x(),p1.x());
            double right=max(p0.x(),p1.x());
            double top=min(p0.y(),p1.y());
            double bottom=max(p0.y(),p1.y());
            if (minx>left && maxx<right && miny>top && maxy<bottom) return true;
            else return false;

    }
    void virtual updateRange() =0;
    void  drag(QPointF point)
    {
        for (int i=0;i<points.size();i++){
            points[i].setX(points.at(i).x()+point.x());
            points[i].setY(points.at(i).y()+point.y());

        }
        updateRange();
    }
    bool  isEmpty(){
        return false;
    }
//    qreal rotationangle(){
//        return Rotationangle;
//    }
//    void setRotationangle(qreal angle){
//        Rotationangle=angle;
//    }

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
 
protected:    
 
    //QPoint beginPoint;
    //QPoint endPoint;

    //QPen pen;
public:
//    QVector<QPointF> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
//    qreal Rotationangle;
//    qreal sx,sy;
//    QBrush brush;


};


 
#endif // ABSTRACTSHAPE_H
