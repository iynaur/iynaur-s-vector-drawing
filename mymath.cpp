#include<mymath.h>

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
