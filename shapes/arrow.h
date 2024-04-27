#ifndef ARROW_H
#define ARROW_H

#include "abstractshape.h"

class DLL Arrow : public AbstractShape
{
public:
    Arrow();
public:
    //QString virtual qStringFromThis();
    QString name() {return "arrow";}
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
    int m_id;
};

#endif // ARROW_H
