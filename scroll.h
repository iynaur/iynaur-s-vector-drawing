#ifndef SCROLL_H
#define SCROLL_H
#include <QScrollArea>
#include "drawareawidget.h"
class Scroll : public QScrollArea
{
public:
    Scroll();
    ~Scroll();
    DrawAreaWidget * drawAreaWidget;
    void closeEvent(QCloseEvent *event);
public slots:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
};

#endif // SCROLL_H
