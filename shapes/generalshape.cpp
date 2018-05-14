#include "generalshape.h"
#include<QPainter>
GeneralShape::GeneralShape()


{
    points=QVector<QPointF>(0);
}
GeneralShape::~GeneralShape(){

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
        appendPoint(QPointF(x,y));
        a=b;
    }
}


QPointF GeneralShape:: centralPoint(){
    return QPointF((minx+maxx)/2,(miny+maxy)/2);
}

void GeneralShape:: zoom(qreal zoomratio){

    updateRange();
}
void GeneralShape:: zoom(qreal zx,qreal zy){

    updateRange();
}

//void GeneralShape::scale(QPointF from, QPointF to, QPointF center)
//{
//	auto vectorFrom = rotated(from - center, rotationangle() / 180 * M_PI);
//	auto vectorTo = rotated(to - center, rotationangle() / 180 * M_PI);
//	double cdsx = vectorTo.x() / vectorFrom.x();
//	double cdsy = vectorTo.y() / vectorFrom.y();
//	auto oldsx = getsx();
//	auto oldsy = getsy();
//	auto oldCenter = centralPoint();
//	if(cdsx!=0.0)setsx(cdsx*oldsx);
//	
//	if (cdsy != 0.0)setsy(cdsy*oldsy);
//	auto dsy = getsy() / oldsy;
//	
//	auto rc = rotated(oldCenter - center, rotationangle() / 180 * M_PI);
//	auto rnewCenter = QPointF(rc.x()*cdsx, rc.y()*dsy);
//	auto newCenter = rotated(rnewCenter, -rotationangle() / 180 * M_PI)+ center;
//	drag(newCenter - oldCenter);
//}

void GeneralShape::scale(int pid, QPointF to)
{
	auto ps = scaleHandlePoints();
	auto center = ps[(pid + 2) % 4];
	auto from = ps[pid];
	auto des = rotated(to - center, rotationangle()/180*M_PI);

	double a = (maxx - minx)*(1 - (pid + 1) / 2 % 2 * 2);
	double b = (maxy - miny)*(1 - (pid) / 2 % 2 * 2);

	double csx = des.x() / a;
	double csy = des.y() / b;
	setsy(csy);
	setsx(csx);
	auto realdes = QPointF(a*getsx(), b*getsy());
	realdes = rotated(realdes, -rotationangle() / 180 * M_PI)+center;
	drag((realdes - from) / 2);
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

//void GeneralShape:: drag(QPointF point)
//{

//    updateRange();
//}
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
void GeneralShape:: drawClosure(QPainter &painter, qreal zoomRatio){
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    //QBrush defaultbrush=QBrush(QColor(ARGB 1, 0, 0, 0) , Qt::NoBrush );
    painter.setBrush( QBrush( Qt::NoBrush ));

    painter.translate((minx+maxx)/2*zoomRatio, (miny+maxy)/2*zoomRatio);
    painter.rotate( Rotationangle );
    painter.drawRect((minx-maxx)/2*zoomRatio*sx,(miny-maxy)/2*zoomRatio*sy,
                     (maxx-minx)*zoomRatio*sx,(maxy-miny)*zoomRatio*sy);
    painter.drawLine(QPointF(0,(miny-maxy)/2*zoomRatio*sy) ,
                     QPointF(0,(miny-maxy)/2*zoomRatio*sy-sy/abs(sy)*lenthOfRotationHandleLine));
    painter.setPen(QPen(Qt::black,3));
    painter.drawPoint(-(minx-maxx)/2*zoomRatio*sx,-(miny-maxy)/2*zoomRatio*sy);
    painter.setPen(pen);
    painter.rotate( -Rotationangle );
    painter.translate(-((minx+maxx)/2)*zoomRatio, -((miny+maxy)/2)*zoomRatio);
}
QPointF GeneralShape:: rotationHandlePoint(){
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
QPointF GeneralShape:: scaleHandlePoint(){
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
QVector<QPointF> GeneralShape::scaleHandlePoints()
{
	double left = minx;
	double right = maxx;
	double top = miny;
	double bottom = maxy;
	auto res = QVector<QPointF>();
	double a = (left - right) / 2 * sx;
	double b = (top - bottom) / 2 * sy;
	for (int i = 0; i < 4; i++) {
		double x = a*(1-(i+1)/2%2*2);
		double y = b*(1 - (i) / 2 % 2*2);
		double sita = Rotationangle / 180 * M_PI;
		double x1 = x*cos(sita) - y*sin(sita);
		double y1 = x*sin(sita) + y*cos(sita);
		res.append(QPointF(-x1 + (left + right) / 2, -y1 + (top + bottom) / 2));
	}
	return res;
}
void GeneralShape::mousePress(QPointF p){
    appendPoint(p);
    appendPoint(p);
}

void GeneralShape::mouseMove(QPointF p){
    points.removeLast();
    appendPoint(p);
}

void GeneralShape::mouseRelease(QPointF p){

}
void  GeneralShape::appendPoint(QPointF point) {

        points.append(point);
        updateRange();

}
     //为了画任意曲线特意留的一个口子
void  GeneralShape::removeLastPoint(){
    points.removeLast();
}
