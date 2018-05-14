#ifndef GS_H
#define GS_H
//#include <QWidget>
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
#include<memory>
#include<ishape.h>
//#include<memleak.h>
using namespace std;

class DLL GeneralShape //: public IShape
{
 
public:
    qreal minx,miny,maxx,maxy;
    qreal top,bottom,left,right;
    virtual QString name()=0;
 
     GeneralShape();
    virtual ~GeneralShape();
    virtual shared_ptr<GeneralShape>  copyPaste()=0;


    QString virtual qStringFromPoints()=0;
    void virtual fromline(QString line);

    void virtual draw(QPainter &painter,qreal zoomRatio)=0;//需要具体图形子类实现

         //为了画任意曲线特意留的一个口子

    double virtual minDistance(QPointF point)=0;
    void  virtual drawClosure(QPainter &painter,qreal zoomRatio);
    QPointF virtual rotationHandlePoint();
    QPointF virtual scaleHandlePoint();
	QVector<QPointF> virtual scaleHandlePoints();
    QPointF  centralPoint();

    void virtual zoom(qreal zoomratio);
    void virtual zoom(qreal zx,qreal zy);
	void scale(int pid, QPointF to);

    bool  inRange(QPointF p0,QPointF p1);
    void virtual updateRange() =0;
    virtual void  updateBand();
    void virtual drag(QPointF point)=0;

    bool virtual isEmpty();
    qreal rotationangle();
    void setRotationangle(qreal angle);
    void virtual appendPoint(QPointF point);
    void virtual removeLastPoint();
    void virtual setPen(const QPen &pen)=0;
    void virtual setBrush(const QBrush &brush)=0;
    void virtual setsx(double x);
    void virtual setsy(double y);
    double virtual getsy();
    double virtual getsx();
//    double  sy();
//    double  sx();
    void virtual mousePress(QPointF p);
    void virtual mouseMove(QPointF p);
    void virtual mouseRelease(QPointF p);

    QPen pen;

public:
    QVector<QPointF> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    qreal Rotationangle; //顺时针旋转 角度制

    QBrush brush;
protected:
    qreal sx,sy;
};


 
#endif // GS_H
