#ifndef TEXT
#define TEXT
#include "rect.h"
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>
#include <QGraphicsTextItem>
class Text : public Rect
{
public:
    Text();
    //QString virtual qStringFromThis();
    virtual shared_ptr<GeneralShape > copyPaste();
    void drag(QPointF point);
    void draw(QPainter &painter,qreal zoomRatio);
    double   minDistance(QPointF point);
    void updateRange();
    //bool inRange(QPoint p0,QPoint p1);
    bool isEmpty();
    void setText(QString text);
    void setFont(QFont font);
    QString text();
    //void drawClosure(QPainter &painter, qreal zoomRatio);
    //QPointF rotationHandlePoint();
    //QPointF scaleHandlePoint();
    void setsx(double x);
    void setsy(double y);
    double virtual getsy();
    void  zoom(qreal zoomratio);
    QString name();

protected:

public:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    QString mytext;
    QFont myfont;
    //QGraphicsTextItem *item;
    //QRectF rectitem;
    //qreal rectitemx;
    //qreal rectitemy;
    //int fontSize;

};
#endif // TEXT

