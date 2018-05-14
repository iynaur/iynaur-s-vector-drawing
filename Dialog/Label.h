#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class CLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CLabel(QWidget *parent = 0);
    bool eventFilter(QObject *target,QEvent *event);
protected:
   void mousePressEvent(QMouseEvent *ev);
signals:
   void sigClicked();
public slots:
    
};

#endif // LABEL_H
