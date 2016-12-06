#include "LineEdit.h"
#include <QDir>
#include <QDebug>
#include <QEvent>
CLineEdit::CLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
  //  this->setFrame(false);
    m_bFocus = false;
    lineHeight=35;
    initWidgets();
    installEventFilter(this);
    setFixedHeight(lineHeight);
    setFont(QFont("Times", 20, QFont::Normal));
    this->setStyleSheet(tr("padding:0px %1px 0px 0px;").arg(lineHeight));//留一点右边的空间用于放置删除内容的控件
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(sltTextChanged(QString)));
    connect(m_qpClearLbl,SIGNAL(sigClicked()),this,SLOT(clear()));
}
void CLineEdit::initWidgets()
{
   m_qpClearLbl=new CLabel(this);  
   QPixmap  pixmap;
   //qDebug()<<"currentPath"<<QDir::currentPath();
   pixmap.load(":/image/clear.png");
   //pixmap.load(QDir::currentPath()+"/images/clear.png");
   QPixmap newPixmap = pixmap.scaled(QSize(lineHeight,lineHeight),Qt::KeepAspectRatio,Qt::SmoothTransformation);
   m_qpClearLbl->setPixmap(newPixmap);
   m_qpClearLbl->hide();
}

void CLineEdit::resizeEvent(QResizeEvent *e)
{
   // qDebug()<<"width"<<this->width()<<"height"<<this->height();
    m_qpClearLbl->move(this->width()-lineHeight,0);
    QWidget::resizeEvent(e);
}
void CLineEdit::sltTextChanged(const QString &str)
{
    if(str.isEmpty())
    {
        m_qpClearLbl->hide();
    }
    else
    {
        m_qpClearLbl->show();
    }
}

bool CLineEdit::eventFilter(QObject *target,QEvent *event)
{

    if( event->type() == QEvent::FocusIn)
    {
        if(!this->text().isEmpty())
        {
            m_qpClearLbl->show();
        }
       m_bFocus = true;
    }
    if(event->type() == QEvent::FocusOut)
    {
        m_qpClearLbl->hide();
        m_bFocus = false;
    }
    return QWidget::eventFilter(target,event);//return QLineEdit::eventFilter(target,event);
}



