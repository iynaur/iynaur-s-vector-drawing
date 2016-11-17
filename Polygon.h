#ifndef PG_H
#define PG_H
 
#include "Poly.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class Polygon : public Poly
{
public:
    Polygon();
    virtual Polygon *  copyPaste(){
        Polygon* tmp=new Polygon;//这里会出问题，closercurve 对象copyPaste后可能变成 polygon;
        copypoly(tmp);//这里会出问题，closercurve 对象copyPaste后可能变成 polygon,但name仍然被赋值为closercurve;
        return tmp;
    }
    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Polygon"+qStringFromPoints();
    }
    double  virtual minDistance(QPointF point){

        //qDebug()<<"points.size()="<<points.size();
        double left=minx;
        double right=maxx;
        double top=miny;
        double bottom=maxy;
        point = point-QPointF((left+right)/2, (top+bottom)/2);
        double sita=Rotationangle/180*M_PI;

        double x=point.x()*cos(sita)+point.y()*sin(sita);
        double y=-point.x()*sin(sita)+point.y()*cos(sita);
        point=QPointF(x,y);
        //point=point+QPointF((left+right)/2, (top+bottom)/2);
        double p0[2]={point.x(),point.y()};
        double min=MAX;
        Polygon *tmp=new Polygon;
        tmp->points=points;
        tmp->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
        tmp->zoom(sx,sy);
        if (QPolygonF(tmp->points).containsPoint(point,Qt::OddEvenFill)) return 0;
        for (int i=0;i<tmp->points.size()-1;i++){

            double p1[2]={tmp->points.at(i).x(),tmp->points.at(i).y()};
            double p2[2]={tmp->points.at(i+1).x(),tmp->points.at(i+1).y()};
            if (CalculatePointToLineDistance(  p0,  p1,  p2) <min){
                min=CalculatePointToLineDistance(  p0,  p1,  p2);
            }
        }
        double p1[2]={tmp->points.at(tmp->points.size()-1).x(),tmp->points.at(tmp->points.size()-1).y()};
        double p2[2]={tmp->points.at(0).x(),tmp->points.at(0).y()};
        if (CalculatePointToLineDistance(  p0,  p1,  p2) <min){
            min=CalculatePointToLineDistance(  p0,  p1,  p2);
        }
        //qDebug()<<"min="<<min;
        return min;
    }

//    double   minDistance(QPointF point){
//        //qDebug()<<"points.size()="<<points.size();
//        double min=MAX;
//        for (int i=0;i<points.size()-1;i++){
//            double p0[2]={point.x(),point.y()};
//            double p1[2]={points.at(i).x(),points.at(i).y()};
//            double p2[2]={points.at(i+1).x(),points.at(i+1).y()};
//            if (CalculatePointToLineDistance(  p0,  p1,  p2) <min){
//                min=CalculatePointToLineDistance(  p0,  p1,  p2);
//            }


//        }
//        double p0[2]={point.x(),point.y()};
//        double p1[2]={points.at(points.size()-1).x(),points.at(points.size()-1).y()};
//        double p2[2]={points.at(0).x(),points.at(0).y()};
//        if (CalculatePointToLineDistance(  p0,  p1,  p2) <min){
//            min=CalculatePointToLineDistance(  p0,  p1,  p2);
//        }
//        qDebug()<<"min="<<min;
//        return min;
//    }
    void draw(QPainter &painter,qreal zoomRatio);

    bool virtual isEmpty(){
        if (points.size()<3) return true;
        else return false;
    }


 
protected:
 
private:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    //QPoint curpoint;

};
 
#endif //BK_H
