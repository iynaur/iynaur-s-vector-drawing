#include "arrow.h"
#include<QPainter>

#include<cmath>



void Arrow::draw(QPainter &painter, qreal zoomRatio)
{
    painter.setPen(pen);
    painter.setBrush(brush);
    double left=(points.at(0).x());
    double right=(points.at(1).x());
    double top=(points.at(1).y());
    double bottom=(points.at(0).y());
    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );
    {
    painter.save();
    painter.drawLine((right - left)/2*zoomRatio*sx,(top-bottom)/2*zoomRatio*sy,(left - right)/2*zoomRatio*sx,(bottom-top)/2*zoomRatio*sy);
    painter.translate((right - left)/2*zoomRatio, (top-bottom)/2*zoomRatio); // to end
    painter.setFont(QFont("ËÎÌו", 15));
     painter.drawText(2, 2, QString::number(m_id));
    {
        painter.save();
        painter.rotate(std::atan2( (top-bottom)/2*zoomRatio, (right - left)/2*zoomRatio) / M_PI * 180); // align with line
        int arrow_len = 5;
        painter.drawLine(0, 0, -arrow_len, arrow_len);
        painter.drawLine(0, 0, -arrow_len, -arrow_len);

        painter.restore();
    }
    painter.restore();
    }

    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);
}





void Arrow::removeLastPoint(){
    points.removeLast();
}
double Arrow::minDistance(QPointF point){//need re-caculate

    double x0=(maxx+minx)/2;
    double y0=(maxy+miny)/2;
    double a=(maxx-minx)/2*abs(sx);
    double b=(maxy-miny)/2*abs(sy);
    double x=point.x()-x0;
    double y=point.y()-y0;
    //recaculate position in translated Coordinate System
    //todo

    qreal sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)+y*sin(sita);
    double y1=-x*sin(sita)+y*cos(sita);
    x=x1;y=y1;

    return  max(( sqrt( (x/a)*(x/a) + (y/b)*(y/b) ) - 1 )*(a+b)/2 , 0.0) ;

}
void Arrow::drag(QPointF point){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
    updateRange();
    //qDebug()<<"moved"<<(point);
}
void Arrow::updateRange(){
    if (points.size()<2) return;
    minx=min(points.at(0).x(),points.at(1).x());
    maxx=max(points.at(0).x(),points.at(1).x());
    miny=min(points.at(0).y(),points.at(1).y());
    maxy=max(points.at(0).y(),points.at(1).y());
    updateBand();

}


Arrow::Arrow()
{
    static int id = 0;
    m_id = id++;
}

shared_ptr<GeneralShape> Arrow::copyPaste(){
     shared_ptr<Arrow> tmp=shared_ptr<Arrow>(new Arrow);
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
 bool Arrow:: isEmpty(){
     if (points.size()>=2) return false;
     else return true;
 }

