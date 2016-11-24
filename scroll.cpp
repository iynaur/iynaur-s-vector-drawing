#include "scroll.h"

Scroll::Scroll()
{
    drawAreaWidget = new DrawAreaWidget(this);
    drawAreaWidget->setBackgroundRole(QPalette::Base);
    drawAreaWidget->vBar=verticalScrollBar();
    drawAreaWidget->hBar=horizontalScrollBar();
    drawAreaWidget->setFixedSize(700,600);
    drawAreaWidget->windowwidth=width();
    drawAreaWidget->windowheight=height();
    setBackgroundRole(QPalette::Dark);
    setWidget(drawAreaWidget);
    setAlignment(Qt::AlignCenter);
    //setFixedSize(700,500);
    //setWindowState(Qt::WindowMaximized);
}
Scroll::~Scroll(){
    delete drawAreaWidget;
}

void Scroll::closeEvent(QCloseEvent *event){
    //qDebug()<<"close";
    if(drawAreaWidget->maybeSave())
        event->accept();
    else
        event->ignore();
}
