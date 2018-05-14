#include "Label.h"
#include <QMouseEvent>
#include <QCursor>
#include <QDebug>
CLabel::CLabel(QWidget *parent) :
    QLabel(parent)
{
    installEventFilter(this);
}
void CLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton)
    {
        emit sigClicked();
    }

    QLabel::mousePressEvent(ev);
}
bool CLabel::eventFilter(QObject *target,QEvent *event)
{

    if(event->type() == QEvent::Enter)
    {
       this->setCursor(QCursor(Qt::PointingHandCursor));
    }
    return QWidget::eventFilter(target,event);
}

