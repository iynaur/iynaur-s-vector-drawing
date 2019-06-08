#ifndef ROTATEWIDGET_H
#define ROTATEWIDGET_H

#include "dialog_global.h"
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include<cmath>
#include"qmath.h"
#include"mywidget.h"
#include"mymath.h"
class DIALOG_EXPORT RotateWidget : public QWidget
{
    Q_OBJECT
public:
    RotateWidget(QWidget *parent = nullptr): QWidget(parent){
        m_ang=90;
        m_baseWidget=0;
    }

    QWidget *baseWidget() const
    {
        return m_baseWidget;
    }
    void setBaseWidget(MyLineEdit *baseWidget)
    {
        m_baseWidget = baseWidget;
        //m_baseWidget->show();
        //connect(m_baseWidget,SIGNAL(painted()),this,SLOT(update()));
    }
    void setRotation(qreal ang){
        m_ang=ang;
    }
    void draw(QPainter &p,qreal zoomRatio){
        p.save();
        if(!m_baseWidget)
            return ;
        QPixmap basePixmap(m_baseWidget->size());
//        m_baseWidget->setFocus();
//        m_baseWidget->activateWindow();
        m_baseWidget->render(&basePixmap);
        //QPainter p(parentWidget());
        p.setRenderHint(QPainter::Antialiasing);
        //p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.setRenderHint(QPainter::HighQualityAntialiasing);
        p.translate(m_baseWidget->pos());
        p.translate(m_baseWidget->width()/2.0,m_baseWidget->height()/2.0);
        p.scale(zoomRatio,zoomRatio);
        p.rotate(m_ang+(m_baseWidget->isReverse?180.0:0.0));//顺时针--
        //if (m_baseWidget->isReverse) p.rotate(180);
        p.translate(-m_baseWidget->width()/2.0,-m_baseWidget->height()/2.0);
        p.drawPixmap(0,0,basePixmap);
        p.setBrush(Qt::SolidPattern);
        p.setPen(QPen());
        QRect rect=m_baseWidget->cursorRect();
        p.drawRect(rect.x()+(rect.width()-1)/2,rect.y(),2,rect.height());
        p.restore();
        //p.end();
    }
    void contextMenu(QContextMenuEvent * event){
        QMenu* menu=m_baseWidget->createStandardContextMenu();
        //m_baseWidget->contextMenuEvent(event);
        menu->exec(event->globalPos());
    }

protected:

public:
    void keyPressEvent(QKeyEvent * event){
        if (m_baseWidget==0) {
            event->ignore();
            return;}
        m_baseWidget->keyPressEvent(event);
        //m_baseWidget->show();
        //m_baseWidget->lower();
        //m_baseWidget->update();
        //m_baseWidget->hide();
        update();
    }
    void mousePress(QMouseEvent * event, qreal zoomRatio){
        if (m_baseWidget==0) {
            event->ignore();
            return;}
        QPointF cenp=m_baseWidget->pos()+QPointF(m_baseWidget->width()/2,m_baseWidget->height()/2);
        //rotated(event->pos()-cenp,  m_ang/180*M_PI)-m_baseWidget->pos();


        QMouseEvent* e=new QMouseEvent(event->type(),
                                       QPointF(rotated(event->pos()-cenp,  (m_ang+(m_baseWidget->isReverse?180:0))/180*M_PI)/zoomRatio+cenp-m_baseWidget->pos()
                                               ),
                                       event->button(),event->buttons(),event->modifiers());
        m_baseWidget->mousePressEvent(e);

        m_baseWidget->update();
    }
    void mouseMove(QMouseEvent * event,qreal zoomRatio){
        if (m_baseWidget==0) {
            event->ignore();
            return;}
        QPointF cenp=m_baseWidget->pos()+QPointF(m_baseWidget->width()/2,m_baseWidget->height()/2);
        //rotated(event->pos()-cenp,  m_ang/180*M_PI)-m_baseWidget->pos();


        QMouseEvent* e=new QMouseEvent(
			event->type(),
            rotated(
				event->pos()-cenp,  
				(m_ang+(m_baseWidget->isReverse?180:0))/180*M_PI
			)/zoomRatio+cenp-m_baseWidget->pos(),
            event->button(),
			event->buttons(),
			event->modifiers());
        m_baseWidget->mouseMoveEvent(e);

        update();
    }
private:
    MyLineEdit* m_baseWidget;
    qreal m_ang;//角度制--
};

#endif // ROTATEWIDGET_H
