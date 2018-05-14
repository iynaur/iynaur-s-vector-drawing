#ifndef MYWIDGET_H
#define MYWIDGET_H
#include "dialog_global.h"
#include <QLineEdit>
#include<QKeyEvent>
class DIALOG_EXPORT MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    //explicit MyLineEdit(QWidget *parent = 0);


    explicit MyLineEdit(QWidget *parent=0) : QLineEdit(parent)
    {
        isReverse=false;
    }
    void keyPressEvent(QKeyEvent *event){
        QLineEdit::keyPressEvent(event);
        //event->ignore();
    }
    void paintEvent(QPaintEvent *e){
        QLineEdit::paintEvent(e);
        emit painted();
    }
    void mousePressEvent(QMouseEvent* e){
        QLineEdit::mousePressEvent( e);
    }
    void mouseMoveEvent(QMouseEvent* e){
        QLineEdit::mouseMoveEvent( e);
    }
    bool eventFilter(QObject * object, QEvent * event)
    {
            if (event->type() == QEvent::FocusOut) return true;

            return QLineEdit::eventFilter(object, event);
    }
    void setReverse(bool rev){
        isReverse=rev;
    }
    QRect cursorRect(){
        return QLineEdit::cursorRect();
    }
    void contextMenuEvent(QContextMenuEvent *event){
        QLineEdit::contextMenuEvent(event);
    }

    bool isReverse;
signals:
    void painted();
public slots:
};

#endif // MYWIDGET_H
