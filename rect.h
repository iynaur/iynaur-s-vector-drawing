#ifndef RECT
#define RECT
#include "abstractshape.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class Rect : public AbstractShape
{
public:
    Rect();
    //QString virtual qStringFromThis();
    QString name();
    virtual shared_ptr<GeneralShape >  copyPaste();
    bool virtual isEmpty();
    //void addPoint(QPointF point);
    void drag(QPointF point);
    void virtual draw(QPainter &painter,qreal zoomRatio);
    //void removeLastPoint();
    double virtual   minDistance(QPointF point);
    void  updateRange();
    //bool inRange(QPointF p0, QPointF p1);
    //void virtual drawClosure(QPainter &painter, qreal zoomRatio);
    //QPointF virtual rotationHandlePoint();
    //QPointF  scaleHandlePoint();

protected:


    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
};
#endif // RECT

