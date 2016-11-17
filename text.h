#ifndef TEXT
#define TEXT
#include "rect.h"
#include <Qpainter>
#include <QVector>
#include <QPointF>
#include <QPolygon>

class Text : public Rect
{
public:
    Text();
    QString virtual qStringFromThis(){
        //qDebug()<<"name="<<metaObject()->className();
        return "Text"+qStringFromPoints()+mytext;
    }
    virtual Text *  copyPaste(){
        Text* tmp=new Text;
        tmp->points=points;
        tmp->name=name;
        tmp->pen=pen;
        tmp->Rotationangle=Rotationangle;
        tmp->sx=sx;
        tmp->sy=sy;
        tmp->brush=brush;
        tmp->mytext=mytext;
        tmp->myfont=myfont;
        tmp->updateRange();//这一步不能少！！！！！！
        return tmp;

    }
    void drag(QPointF point);
    void draw(QPainter &painter,qreal zoomRatio);
    double   minDistance(QPointF point);
    void updateRange();
    bool inRange(QPoint p0,QPoint p1);
    void setText(QString text);
    void setFont(QFont font){
        myfont=font;
        updateRange();
    }
    QString text(){
        return mytext;
    }
    void drawClosure(QPainter &painter, qreal zoomRatio){
        QPen pen;  // creates a default pen

        pen.setStyle(Qt::DashDotLine);
        painter.setPen(pen);
        painter.setBrush( QBrush( Qt::NoBrush ));
        double left=minx;
        double right=maxx;
        double top=miny;
        double bottom=maxy;
        painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
        painter.rotate( Rotationangle );

        painter.drawRect((left-right)/2*zoomRatio*sx,(top-bottom)/2*zoomRatio*sx,
                         (right-left)*zoomRatio*sx,(bottom-top)*zoomRatio*sx);
        painter.drawLine(QPointF(0,(top-bottom)/2*zoomRatio*sx) ,
                         QPointF(0,(top-bottom)/2*zoomRatio*sx-sx/abs(sx)*lenthOfRotationHandleLine));
        painter.setPen(QPen(Qt::black,3));
        painter.drawPoint(-(left-right)/2*zoomRatio*sx,-(top-bottom)/2*zoomRatio*sx);
        painter.setPen(pen);
        painter.rotate( -Rotationangle );
        painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);
    }
    QPointF rotationHandlePoint(){
            double left=minx;
            double right=maxx;
            double top=miny;
            double bottom=maxy;
        //double x=0;
        double y=(top-bottom)/2*sx;
        double x1=-y*sin(Rotationangle/180*M_PI);
        double y1=y*cos(Rotationangle/180*M_PI);
        return QPointF(x1+(left+right)/2,y1+(top+bottom)/2);

    }
    QPointF scaleHandlePoint(){
            double left=minx;
            double right=maxx;
            double top=miny;
            double bottom=maxy;
        double x=(left-right)/2*sx;
        double y=(top-bottom)/2*sx;
        double sita=Rotationangle/180*M_PI;
        double x1=x*cos(sita)-y*sin(sita);
        double y1=x*sin(sita)+y*cos(sita);
        return QPointF(-x1+(left+right)/2,-y1+(top+bottom)/2);
    }
    void virtual setsx(double x){
        sx=x;
        sy=x;
    }
    void virtual setsy(double y){
        //do nothing
    }
    double virtual getsy(){
        return sx;
    }
    void  zoom(qreal zoomratio){
        for (int i=0;i<points.size();i++){
            points[i].setX((points.at(i).x()+0.5*(maxx-minx))*zoomratio-0.5*(maxx-minx));
            points[i].setY((points.at(i).y()-0.5*(maxy-miny))*zoomratio+0.5*(maxy-miny));

        }

        setsx(sx*zoomratio);
        updateRange();
    }

protected:

public:
    //QVector<QPoint> points; //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    QString mytext;
    QFont myfont;

};
#endif // TEXT

