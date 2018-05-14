//#define DLL_IMPLEMENT
#include<mymath.h>
#include<generalshape.h>

double distance(QPointF p1, QPointF p2)
{
	return abs(p1.x()- p2.x())+ abs(p1.y() - p2.y());
}

double CalculatePointToLineDistance(QPointF pzero, const QPointF pone, const QPointF ptwo)
{
	double p0[2] = { pzero.x(),pzero.y() };
	double p1[2] = { pone.x(),pone.y() };
	double p2[2] = { ptwo.x(),ptwo.y() };
	return CalculatePointToLineDistance(p0, p1, p2);
}

double CalculatePointToLineDistance(double point[2], const double p1[2], const double p2[2])
{
    double dis = 0.f;

    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];

    // 两直线垂直，向量表示法，转换后公示
    double k = -((p1[0] - point[0])*dx + (p1[1] - point[1])*dy) / ( dx*dx + dy*dy);
    double footX = k*dx + p1[0];
    double footY = k*dy + p1[1];

    //if垂足是否落在线段上
    if( footY >= min(p1[1], p2[1]) && footY <=max(p1[1], p2[1])
        && footX >= min(p1[0], p2[0]) && footX <=max(p1[0], p2[0] ) )
    {
        dis = sqrt((footX-point[0])*(footX-point[0]) + (footY-point[1])*(footY-point[1]));
    }
    else
    {
        double dis1 = sqrt((p1[0]-point[0])*(p1[0]-point[0]) + (p1[1]-point[1])*(p1[1]-point[1]));
        double dis2 = sqrt((p2[0]-point[0])*(p2[0]-point[0]) + (p2[1]-point[1])*(p2[1]-point[1]));

        dis = ( dis1 < dis2 ? dis1 : dis2 );
    }

    return dis;
}
QPointF rotated(QPointF p,qreal sita){//计算机绘图坐标系，点逆时针旋转（或坐标系顺时针旋转），弧度制
    qreal x1=p.x()*cos(sita)+p.y()*sin(sita);
    qreal y1=-p.x()*sin(sita)+p.y()*cos(sita);
    return QPointF(x1,y1);
}
bool isScaleHandlePoint(QPointF realPoint, shared_ptr<GeneralShape> pickedShape, double zoomRatio) {
	QPointF handlePoint = pickedShape->scaleHandlePoint();
	//qDebug()<<handlePoint;
	double x = handlePoint.x();
	double y = handlePoint.y();
	//qDebug()<<x-realPoint.x()<<y-realPoint.y();
	if (abs(x - realPoint.x())*zoomRatio < 5 && abs(y - realPoint.y())*zoomRatio < 5) return true;
	else return false;
}

int DLL scaleHandlePointId(QPointF realPoint, shared_ptr<GeneralShape> pickedShape, double zoomRatio)
{
	auto handlePoints = pickedShape->scaleHandlePoints();
	for (int i = 0; i < handlePoints.size();++i) {
		auto handlePoint = handlePoints[i];
		double x = handlePoint.x();
		double y = handlePoint.y();
		//qDebug()<<x-realPoint.x()<<y-realPoint.y();
		if (abs(x - realPoint.x())*zoomRatio < 5 && abs(y - realPoint.y())*zoomRatio < 5) return i;
	}
	return -1;
}

bool isRotationHandlePoint(QPointF realPoint, shared_ptr<GeneralShape> pickedShape, double zoomRatio) {
	QPointF handlePoint = pickedShape->rotationHandlePoint();
	//qDebug()<<handlePoint;
	double sy = pickedShape->getsy();
	double x = handlePoint.x() + sy / abs(sy)*lenthOfRotationHandleLine / zoomRatio*sin(pickedShape->rotationangle() / 180 * M_PI);
	double y = handlePoint.y() - sy / abs(sy)*lenthOfRotationHandleLine / zoomRatio*cos(pickedShape->rotationangle() / 180 * M_PI);
	//qDebug()<<x-realPoint.x()<<y-realPoint.y();
	if (abs(x - realPoint.x())*zoomRatio < 5 && abs(y - realPoint.y())*zoomRatio < 5) return true;
	else return false;
}