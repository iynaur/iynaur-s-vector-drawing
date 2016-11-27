#include "scroll.h"

Scroll::Scroll()
{
    drawAreaWidget = new DrawAreaWidget(this);
    drawAreaWidget->setBackgroundRole(QPalette::Base);
    drawAreaWidget->vBar=verticalScrollBar();
    drawAreaWidget->hBar=horizontalScrollBar();
    drawAreaWidget->setFixedSize(900,500);
    drawAreaWidget->windowwidth=width();
    drawAreaWidget->windowheight=height();
    setBackgroundRole(QPalette::Dark);
    setWidget(drawAreaWidget);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);

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
void Scroll::dragEnterEvent(QDragEnterEvent *event)//拖进事件

{
    if(event->mimeData()->hasFormat("text/uri-list")){
        event->setDropAction(Qt::IgnoreAction);
        event->acceptProposedAction();
    }
}
void Scroll::dropEvent(QDropEvent *event)//放下事件
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
    {
        return;
    }
    foreach(QUrl url,urls)
    {
       QString fileName = url.toLocalFile();
        if (fileName.isEmpty())
        {
            return;
        }
        else{
            //on_actionNew_triggered();
            drawAreaWidget->openasadd=false;

                if (!drawAreaWidget->maybeSave()) return;
                drawAreaWidget->init();

            drawAreaWidget->openfile(fileName);
        }
    }
}
