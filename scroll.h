#ifndef SCROLL_H
#define SCROLL_H
#include <QScrollArea>
#include "drawareawidget.h"
class Scroll : public QScrollArea
{
public:
    Scroll();

    DrawAreaWidget * drawAreaWidget;
    void closeEvent(QCloseEvent *event);
};

#endif // SCROLL_H
