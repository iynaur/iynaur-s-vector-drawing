#include "ruler.h"

Ruler::Ruler(QMainWindow *parent) : QMainWindow(parent)
{
    ruler=20;
    scrollArea=new Scroll();

    //scrollArea->move(ruler,ruler);
    //frame =new  QFrame;
    //setCentralWidget(frame);
    //setCentralWidget(scrollArea);
    //layout =new QHBoxLayout(this);
    //setLayout(layout);
    scrollArea->setParent(this);
    //layout->addWidget(scrollArea);
    scrollArea->setGeometry(QRect(ruler,ruler,width(),height()));
    //scrollArea->setFixedSize(this->width(),this->height());
    qDebug()<<width()<<height();
}
//void Ruler::paintEvent(QPaintEvent *){

//    QPainter painter(this);
//    painter.drawRect(0,ruler,ruler,height());
//    qDebug()<<width()<<height();

//}
void Ruler::resizeEvent(QResizeEvent *e){
    //scrollArea->resizeEvent(e);
    scrollArea->setGeometry(QRect(ruler,ruler,width(),height()));
    //scrollArea->setFixedSize(width(),height());
    qDebug()<<width()<<height();
}
