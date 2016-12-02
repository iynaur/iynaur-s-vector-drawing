/*********************************/
// 如果经过点做直线的垂足，垂足落在线段上，则取垂线段的距离
// 否则取到线段两端点距离的最小值
//
// 参数：
// point:  存储点的xy坐标
// p1, p2: 线段的两点
//
// return: 点到线段的最小距离
/*********************************/
#ifndef mymh
#define mymh
#include<cmath>
#include <algorithm>
#include <QPointF>
//#include"generalshape.h"
//#include"combo.h"
using namespace std;
double CalculatePointToLineDistance(double point[2], const double p1[2], const double p2[2]);
QPointF rotated(QPointF p, qreal sita);//计算机绘图坐标系，点逆时针旋转（或坐标系顺时针旋转），弧度制

#endif
