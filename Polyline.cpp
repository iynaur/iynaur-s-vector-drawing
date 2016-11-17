#include "Polyline.h"

Polyline::Polyline()
{
    name="Polyline";

}

//void Polyline::draw(QPainter &painter, qreal zoomRatio)
//{
//    painter.setPen(pen);
//    painter.setBrush(brush);
//    double left=minx;
//    double right=maxx;
//    double top=miny;
//    double bottom=maxy;

//    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
//    painter.rotate( Rotationangle );

//    Polyline *tmp=new Polyline;
//    tmp->points=points;
//    tmp->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
//    tmp->zoom(zoomRatio);

//    painter.drawPolyline(QPolygonF(tmp->points));
//    painter.rotate( -Rotationangle );
//    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);


//}


