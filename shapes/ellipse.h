#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "abstractshape.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class DLL Ellipse : public AbstractShape
{
public:
    Ellipse();
    //QString virtual qStringFromThis();
    QString name();
    virtual shared_ptr<GeneralShape>  copyPaste();
    bool virtual isEmpty();
    void drag(QPointF point);
    void draw(QPainter &painter,qreal zoomRatio);
    void removeLastPoint();
    double   minDistance(QPointF point);
    void updateRange();
    //bool inRange(QPoint p0,QPoint p1);
    //void virtual drawClosure(QPainter &painter, qreal zoomRatio);
    //QPointF virtual rotationHandlePoint();
    //QPointF scaleHandlePoint();

protected:

private:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
};
#endif // ELLIPSE

