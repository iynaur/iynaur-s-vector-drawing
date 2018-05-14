#ifndef PL_H
#define PL_H
 
#include "abstractshape.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
 
class DLL Polyline : public AbstractShape
{
public:
    Polyline();
    QString name();
    //QString virtual qStringFromThis();
    void updateRange();
    void  drag(QPointF point);
//    void  removeLastPoint(){
//        points.removeLast();
//    }
    //QString virtual qStringFromThis();
    //void copypolyline(Polyline* tmp);
    virtual shared_ptr<GeneralShape >  copyPaste();
    void virtual draw(QPainter &painter,qreal zoomRatio);
    double  virtual minDistance(QPointF point);
    bool virtual isEmpty();
    //QPointF virtual rotationHandlePoint();
    //QPointF virtual scaleHandlePoint();
    void virtual drawClosure(QPainter &painter, qreal zoomRatio);
	void moveKeyPoint(int id, QPointF dp);
	void delPoint(int id);
	void addPoint(int id, QPointF vP);
	QPointF getVisualPoint(QPointF storedPoint);
};
 
#endif //BK_H
