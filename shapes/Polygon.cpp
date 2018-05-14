#include "polygon.h"

Polygon::Polygon()
{
    //name="Polygon";

}
QString Polygon::name(){
    return "Polygon";
}

void Polygon::draw(QPainter &painter, qreal zoomRatio)
{
    painter.setPen(pen);
    painter.setBrush(brush);
    //qDebug()<<brush.color();
    //painter.setBrush(Qt::red);

    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;

    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );

    Polygon *tmp=new Polygon;
    tmp->points=points;
    tmp->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
    tmp->zoom(zoomRatio*sx,zoomRatio*sy);

    painter.drawPolygon(QPolygonF(tmp->points));
    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);
    delete tmp;

}

 shared_ptr<GeneralShape> Polygon::copyPaste(){
     shared_ptr<Polygon> tmp=shared_ptr<Polygon>(new Polygon);
     tmp->points=points;
     //tmp->name=name;
     tmp->pen=pen;
     tmp->Rotationangle=Rotationangle;
     tmp->sx=sx;
     tmp->sy=sy;
     tmp->brush=brush;
     tmp->updateRange();
     return static_pointer_cast<GeneralShape>(tmp);
}
//QString Polygon:: qStringFromThis(){
//    //qDebug()<<"name="<<metaObject()->className();
//    return "Polygon"+qStringFromPoints();
//}
double  Polygon:: minDistance(QPointF point){

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
    if (QPolygonF(tmp->points).containsPoint(point,Qt::OddEvenFill)) {
        delete tmp;
        return 0;
    }
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
    delete tmp;
    //qDebug()<<"min="<<min;
    return min;
}



bool Polygon:: isEmpty(){
    if (points.size()<3) return true;
    else return false;
}
