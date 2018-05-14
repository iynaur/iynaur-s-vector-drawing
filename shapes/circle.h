#ifndef CIRCLE
#define CIRCLE
#include "abstractshape.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class DLL Circle : public AbstractShape
{
public:
    Circle();
    //QString  qStringFromThis();
    QString name();
    virtual shared_ptr<GeneralShape>  copyPaste();
    //void addPoint(QPointF point);
    void drag(QPointF point);
    void draw(QPainter &painter,qreal zoomRatio);
    //void removeLastPoint();
    double   minDistance(QPointF point);
    void updateRange();
    //bool inRange(QPoint p0,QPoint p1);
    //void drawClosure(QPainter &painter,qreal zoomRatio);
    //QPointF  rotationHandlePoint();
    //QPointF  scaleHandlePoint();
    double virtual getsy();
    bool  isEmpty();
    void updateBand();
    void setsx(double x);
    void setsy(double y);

protected:

private:

};
#endif // CIRCLE

