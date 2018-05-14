#ifndef SCROLL_H
#define SCROLL_H
//#include <QScrollArea>
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
	QDRuler* mHorzRuler;
	QDRuler* mVertRuler;
public slots:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void updateruler();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void contextMenuEvent( QContextMenuEvent * event );
};

#endif // SCROLL_H
