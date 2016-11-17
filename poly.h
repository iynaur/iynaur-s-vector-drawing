#ifndef POLY_H
#define POLY_H
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
//#include "Polyline.h"
class Poly : public AbstractShape
{
public:
    Poly();
    void updateRange(){
        if (points.size()==0) return;
        minx=maxx=points.at(0).x();
        miny=maxy=points.at(0).y();
        for (int i=0;i<points.size();i++){

            minx=min(points.at(i).x(),minx);
            miny=min(points.at(i).y(),miny);
            maxx=max(points.at(i).x(),maxx);
            maxy=max(points.at(i).y(),maxy);
        }
        updateBand();
    }
//    void  addPoint(QPoint point){
//        this->points.append(point);
//        updateRange();
//    }
    void  drag(QPointF point){
        for (int i=0;i<points.size();i++){
            points[i].setX(points.at(i).x()+point.x());
            points[i].setY(points.at(i).y()+point.y());

        }
        updateRange();
        //qDebug()<<"moved"<<(point);
    }
//    void  removeLastPoint(){
//        points.removeLast();
//    }
    QString virtual qStringFromThis(){return "";}
    void copypoly(Poly* tmp){
        tmp->points=points;
        tmp->name=name;
        tmp->pen=pen;
        tmp->Rotationangle=Rotationangle;
        tmp->sx=sx;
        tmp->sy=sy;
        tmp->brush=brush;
    }

    virtual Poly *  copyPaste(){
        Poly* tmp=new Poly;
        copypoly(tmp);
        return tmp;
    }
    void virtual draw(QPainter &painter,qreal zoomRatio){
        painter.setPen(pen);
        painter.setBrush(brush);
        double left=minx;
        double right=maxx;
        double top=miny;
        double bottom=maxy;

        painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
        painter.rotate( Rotationangle );

        Poly *tmp=new Poly;
        tmp->points=points;
        tmp->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
        tmp->zoom(zoomRatio*sx,zoomRatio*sy);

        painter.drawPolyline(QPolygonF(tmp->points));
        painter.rotate( -Rotationangle );
        painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);

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
        Poly *tmp=new Poly;
        tmp->points=points;
        tmp->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
        tmp->zoom(sx,sy);
        for (int i=0;i<tmp->points.size()-1;i++){

            double p1[2]={tmp->points.at(i).x(),tmp->points.at(i).y()};
            double p2[2]={tmp->points.at(i+1).x(),tmp->points.at(i+1).y()};
            if (CalculatePointToLineDistance(  p0,  p1,  p2) <min){
                min=CalculatePointToLineDistance(  p0,  p1,  p2);
            }
        }
        //qDebug()<<"min="<<min;
        return min;
    }
    bool virtual isEmpty(){
        if (points.size()<2) return true;
        else return false;
    }
    QPointF virtual rotationHandlePoint(){
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
    QPointF virtual scaleHandlePoint(){
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
    void virtual drawClosure(QPainter &painter, qreal zoomRatio){
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

};

#endif // POLY_H
