#ifndef CIRCLE
#define CIRCLE
#include "abstractshape.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class Circle : public AbstractShape
{
public:
    Circle();
    //QString  qStringFromThis();
    QString name();
    virtual Circle *  copyPaste();
    //void addPoint(QPointF point);
    void drag(QPointF point);
    void draw(QPainter &painter,qreal zoomRatio);
    //void removeLastPoint();
    double   minDistance(QPointF point);
    void updateRange();
    bool inRange(QPoint p0,QPoint p1);
    void  drawClosure(QPainter &painter,qreal zoomRatio);
    QPointF virtual rotationHandlePoint();
    QPointF  scaleHandlePoint();
    double virtual getsy();
    bool  isEmpty();

protected:

private:

};
#endif // CIRCLE

