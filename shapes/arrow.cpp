#include "arrow.h"
#include "polyline.h"
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
        painter.translate((right - left)/2*zoomRatio*sx, (top-bottom)/2*zoomRatio*sy); // to end

        {
            painter.save();
            painter.rotate(std::atan2( (top-bottom)/2*zoomRatio*sy, (right - left)/2*zoomRatio*sx) / M_PI * 180); // align with line
            int arrow_len = 5;
            painter.drawLine(0, 0, -arrow_len, arrow_len);
            painter.drawLine(0, 0, -arrow_len, -arrow_len);
            painter.setFont(QFont("", 15));
            painter.drawText(2, -2, QString::number(m_id));

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
double Arrow::minDistance(QPointF point){

    Polyline tmp;
    AbstractShape& abs = tmp;
    abs = *this;
    return tmp.minDistance(point);

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

