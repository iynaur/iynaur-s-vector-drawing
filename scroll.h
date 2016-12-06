#ifndef SCROLL_H
#define SCROLL_H
#include <QScrollArea>
#include <QGridLayout>
#include "drawareawidget.h"
#include"ruler.h"
class Scroll : public QScrollArea
{
    Q_OBJECT
public:
    Scroll();
    ~Scroll();
    DrawAreaWidget * drawAreaWidget;
    void closeEvent(QCloseEvent *event);
    QDRuler* mHorzRuler,*    mVertRuler;
public slots:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void updateruler();
};

#endif // SCROLL_H
