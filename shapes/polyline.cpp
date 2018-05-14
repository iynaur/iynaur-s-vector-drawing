#include "polyline.h"

Polyline::Polyline()
{
    //name="Polyline";

}
QString Polyline::name(){
    return "Polyline";
}

void Polyline::updateRange(){
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
void  Polyline::drag(QPointF point){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
    updateRange();
    //qDebug()<<"moved"<<(point);
}

 shared_ptr<GeneralShape> Polyline::copyPaste(){
     shared_ptr<Polyline> tmp=shared_ptr<Polyline>(new Polyline);
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
void Polyline:: draw(QPainter &painter,qreal zoomRatio){
    painter.setPen(pen);
    painter.setBrush(brush);
    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;

    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );

    Polyline *tmp=new Polyline();
    tmp->points=points;
    tmp->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
    tmp->zoom(zoomRatio*sx,zoomRatio*sy);

    painter.drawPolyline(QPolygonF(tmp->points));
    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);
    delete tmp;

}
double  Polyline:: minDistance(QPointF point){
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
    Polyline *tmp=new Polyline;
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
    delete tmp;
    //qDebug()<<"min="<<min;
    return min;
}
bool Polyline:: isEmpty(){
    if (points.size()<2) return true;
    else return false;
}

void Polyline::drawClosure(QPainter & painter, qreal zoomRatio)
{
	AbstractShape::drawClosure(painter, zoomRatio);
	/*painter.translate((minx + maxx) / 2 * zoomRatio, (miny + maxy) / 2 * zoomRatio);
	painter.rotate(Rotationangle);
	painter.setPen(QPen(Qt::black, 3));
	auto cp = centralPoint();
	for (auto p : points)
		painter.drawPoint((p - cp).x() * zoomRatio*(getsx()), (p - cp).y() * zoomRatio*(getsy()));
	painter.rotate(-(Rotationangle));
	painter.translate(-((minx + maxx) / 2)*zoomRatio, -((miny + maxy) / 2)*zoomRatio);*/
}

void Polyline::moveKeyPoint(int id, QPointF dp)
{
	auto p = getVisualPoint(points[id]) + dp;
	auto oldcp = centralPoint();
	double sita = -Rotationangle / 180 * M_PI;
	auto un_rot_p = rotated(p - oldcp, -sita);
	auto un_scale_p = QPointF(un_rot_p.x() / getsx(), un_rot_p.y() / getsy());

	points[id] = un_scale_p + oldcp;
	updateRange();
	auto newcp = centralPoint();
	auto rp = points[id] - newcp;
	auto scale_p = QPointF(rp.x() * getsx(), rp.y() * getsy());
	auto rot_p = rotated(scale_p, sita);
	auto d = p - (rot_p + newcp);
	drag(d);
}

void Polyline::delPoint(int id)
{
	QPointF inner_p;
	if (id != 0)inner_p = points[0];
	else inner_p= points[1];
	auto v_in_p = getVisualPoint(inner_p);
	moveKeyPoint(id, getVisualPoint(inner_p) - getVisualPoint(points[id]));
	points.removeAt(id);
}

void Polyline::addPoint(int id, QPointF vP)
{
	QPointF inner_p = points[0];
	points.insert(id, inner_p);
	auto d = vP - getVisualPoint(inner_p);
	moveKeyPoint(id, d);
}

QPointF Polyline::getVisualPoint(QPointF storedPoint)
{
	double sita = -Rotationangle / 180 * M_PI;
	auto cp = centralPoint();
	auto rp = storedPoint - cp;
	auto scale_p = QPointF(rp.x() * getsx(), rp.y() * getsy());
	auto rot_p = rotated(scale_p, sita);
	return rot_p + cp;
}
