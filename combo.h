#ifndef COMBO_H
#define COMBO_H
#include "generalshape.h"
#include <QPainter>
#include <QtXml>
#include "curve.h"
#include "closecurve.h"
#include "circle.h"
#include "polyline.h"
#include "polygon.h"
#include "rect.h"
#include "ellipse.h"
#include "text.h"

class Combo : public GeneralShape
{
public:
    Combo();
    ~Combo();
    QString name();
    virtual shared_ptr<GeneralShape>  copyPaste();
    void setShapes(QDomElement shapesElement);
    void setShapesFromNode(QDomNode node);
    QDomElement toElement();
    QDomElement toElement(shared_ptr<GeneralShape>sp);
    QDomDocument toDocument();

    //QString virtual qStringFromThis();

    QString virtual qStringFromPoints();

    void virtual draw(QPainter &painter,qreal zoomRatio);



    void virtual zoom(qreal zoomratio);

    double virtual minDistance(QPointF point);
    //void virtual drawClosure(QPainter &painter,qreal zoomRatio);
    //QPointF virtual rotationHandlePoint();

    //QPointF virtual scaleHandlePoint();


    void virtual zoom(qreal zx,qreal zy);


    //bool virtual inRange(QPointF p0,QPointF p1);
    void virtual updateRange() ;

    void virtual drag(QPointF point);
    bool virtual isEmpty();
    qreal rotationangle();
    void setRotationangle(qreal angle);
    void virtual addPoint(QPointF point);
    void virtual removeLastPoint();

    void virtual setPen(const QPen &pen);

    void virtual setBrush(const QBrush &brush);
    void virtual setsx(double x);
    void virtual setsy(double y);
    double virtual getsy();
    //void updatedrawcombo();

//private:
    QList<shared_ptr<GeneralShape>> shapes;
    //Combo* drawcombo;
};

#endif // COMBO_H
