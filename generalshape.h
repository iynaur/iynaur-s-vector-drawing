#ifndef GS_H
#define GS_H
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include<cmath>
#include<mymath.h>
using namespace std;
const double MAX=10000;
const double  lenthOfRotationHandleLine=20;

class GeneralShape : public QWidget
{
    Q_OBJECT
 
public:
    qreal minx,miny,maxx,maxy;
    qreal top,bottom,left,right;
    virtual QString name()=0;
 
    explicit GeneralShape(QWidget *parent = 0);
    virtual GeneralShape *  copyPaste()=0;


    QString virtual qStringFromPoints()=0;
    void virtual fromline(QString line);

    void virtual draw(QPainter &painter,qreal zoomRatio)=0;//需要具体图形子类实现

         //为了画任意曲线特意留的一个口子

    double virtual minDistance(QPointF point)=0;
    void virtual drawClosure(QPainter &painter,qreal zoomRatio)=0;
    QPointF virtual rotationHandlePoint()=0;
    QPointF virtual scaleHandlePoint()=0;
    QPointF virtual centralPoint();

    void virtual zoom(qreal zoomratio);
    void virtual zoom(qreal zx,qreal zy);


    bool  inRange(QPointF p0,QPointF p1);
    void virtual updateRange() =0;
    void virtual updateBand();
    void virtual drag(QPointF point);

    bool virtual isEmpty();
    qreal rotationangle();
    void setRotationangle(qreal angle);
    void virtual addPoint(QPointF point)=0;
    void virtual removeLastPoint()=0;
    void virtual setPen(const QPen &pen)=0;
    void virtual setBrush(const QBrush &brush)=0;
    void virtual setsx(double x);
    void virtual setsy(double y);
    double virtual getsy();
    double virtual getsx();


protected:    
    QPen pen;

public:
    QVector<QPointF> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    qreal Rotationangle;
    qreal sx,sy;
    QBrush brush;

};


 
#endif // GS_H
