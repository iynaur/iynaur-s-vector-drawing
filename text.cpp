#include "text.h"
#include<cmath>
#include <QGraphicsTextItem>
Text::Text()
{
    name="Text";

    mytext="hello world!";
    myfont=QFont("Times", 20, QFont::Normal);
}

//void Text::addPoint(QPoint point)
//{
//    this->points.append(point);
//    updateRange();
//}
void Text::setText(QString text){
    mytext=text;
    updateRange();
}



void Text::draw(QPainter &painter, qreal zoomRatio)
{
    QPen qpen=pen;
    qpen.setColor(brush.color());
    painter.setPen(qpen);
    painter.setBrush(brush);


    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;

    double sita=Rotationangle;
    if (sx<0) sita=sita+180;
    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( sita );

    //tmp->drag(QPoint(dx,dy)/zoomRatio);
    painter.scale(zoomRatio,zoomRatio);

    QGraphicsTextItem *item;
    QFont font=myfont;
    item = new QGraphicsTextItem( 0);//文本的父item为对应的场景
    int i=0;
    do {
        i++;
        font.setPointSize(i);
        item->setFont(font);//为文本设置字体
        item->setHtml(mytext);
    }
    while(item->boundingRect().width()<(maxx-minx)*abs(sx));
    font.setPointSize(i-1);
    painter.setFont(font);
    //drag(QPointF((maxx-minx)/2-item->boundingRect().width()/2,(maxy-miny)/2-item->boundingRect().height()/2));
    painter.drawText(-item->boundingRect().width()/2 , item->boundingRect().height()/2 , mytext);
    //drag(-QPointF((maxx-minx)/2-item->boundingRect().width()/2,(maxy-miny)/2-item->boundingRect().height()/2));
    painter.scale(1/zoomRatio,1/zoomRatio);
    painter.rotate( -sita );
    painter.translate(-((left+right)/2*zoomRatio), -((top+bottom)/2)*zoomRatio);

}
//void Text::removeLastPoint(){
//    points.removeLast();
//}
double Text::minDistance(QPointF point){

    double x0=(maxx+minx)/2;
    double y0=(maxy+miny)/2;
    double a=(maxx-minx)/2*abs(sx);
    double b=a*(maxy-miny)/(maxx-minx);
    double x=point.x()-x0;
    double y=point.y()-y0;
    qreal sita=Rotationangle/180*M_PI;
    double x1=x*cos(sita)+y*sin(sita);
    double y1=-x*sin(sita)+y*cos(sita);
    x=x1;y=y1;


    double min1=min(abs(x-a) , abs(x+a));
    double min2=min(abs(y-b) , abs(y+b));
    if ((x-a)*(x+a) >0){
        if ((y-b)*(y+b)>0)
            return sqrt(min1*min1+min2*min2);
        else
            return min1;

    }else{
        if ((y-b)*(y+b)>0)
            return min2;
        else
            return 0;
    }
}
void Text::drag(QPointF point){
    for (int i=0;i<points.size();i++){
        points[i].setX(points.at(i).x()+point.x());
        points[i].setY(points.at(i).y()+point.y());

    }
    updateRange();
    //qDebug()<<"moved"<<(point);
}
void Text::updateRange(){
    //qDebug()<<"Text::updateRange()";
    QGraphicsTextItem *item;
    item = new QGraphicsTextItem( 0);//文本的父item为对应的场景

    item->setFont(myfont);//为文本设置字体


    item->setHtml(mytext);
    minx=points.at(0).x();
    miny=points.at(0).y()-item->boundingRect().height();
    maxx=points.at(0).x()+item->boundingRect().width();
    maxy=points.at(0).y();
    updateBand();
}
bool Text::inRange(QPoint p0,QPoint p1){
    int left=min(p0.x(),p1.x());
    int right=max(p0.x(),p1.x());
    int top=min(p0.y(),p1.y());
    int bottom=max(p0.y(),p1.y());
    if (minx>left && maxx<right && miny>top && maxy<bottom) return true;
    else return false;
}


