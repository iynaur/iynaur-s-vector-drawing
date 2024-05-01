#include "scroll.h"
//#define RULER_BREADTH 20
Scroll::Scroll()
{
    setViewportMargins(RULER_BREADTH,RULER_BREADTH,0,0);

    // setAutoFillBackground(true);
    //setBackgroundRole(QPalette::Dark);

    // QPalette palette = this->palette();
    // palette.setBrush(QPalette::Dark, QColor("lime"));

    // setPalette(palette);
    setStyleSheet("QScrollArea{background: DarkGray;}");

    drawAreaWidget = new DrawAreaWidget(this);
    drawAreaWidget->setBackgroundRole(QPalette::Light);
    drawAreaWidget->vBar=verticalScrollBar();
    drawAreaWidget->hBar=horizontalScrollBar();
    drawAreaWidget->setFixedSize(drawAreaWidget->realSize);
    drawAreaWidget->windowwidth=width();
    drawAreaWidget->windowheight=height();
    setWidget(drawAreaWidget);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);


    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    mHorzRuler = new QDRuler(QDRuler::Horizontal,this);
    mHorzRuler->setMouseTrack(true);
    mVertRuler = new QDRuler(QDRuler::Vertical,this);
    mVertRuler->setMouseTrack(true);

    QWidget* fake = new QWidget(this);
    fake->setBackgroundRole(QPalette::Window);
    fake->setFixedSize(RULER_BREADTH,RULER_BREADTH);
    gridLayout->addWidget(fake,0,0);
    gridLayout->addWidget(mHorzRuler,0,1);
    gridLayout->addWidget(mVertRuler,1,0);
    gridLayout->addWidget(this->viewport(),1,1);

	//QThread *thread = new QThread;
	//drawAreaWidget->moveToThread(thread);
	//thread->start();

    this->setLayout(gridLayout);

    connect(drawAreaWidget,SIGNAL(statusChanged()),this , SLOT(updateruler()));
    connect(drawAreaWidget,SIGNAL(mouseMoved(QMouseEvent*,QPointF)),mHorzRuler , SLOT(setCursorPos(QMouseEvent*)));
    connect(drawAreaWidget,SIGNAL(mouseMoved(QMouseEvent*,QPointF)),mVertRuler , SLOT(setCursorPos(QMouseEvent*)));

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
void Scroll::updateruler(){
    int hv=horizontalScrollBar()->value();
    int vv=verticalScrollBar()->value();
    if (hv==0){
        hv=hv-drawAreaWidget->geometry().x();
    }
    if (vv==0){
        vv=vv-drawAreaWidget->geometry().y();
    }
    mHorzRuler->setOrigin((-hv+drawAreaWidget->dx));//drawAreaWidget->zoomRatio);
    mVertRuler->setOrigin((-vv+drawAreaWidget->dy));//drawAreaWidget->zoomRatio);
    mHorzRuler->setRulerZoom(drawAreaWidget->zoomRatio);
    mHorzRuler->setRulerUnit(1/drawAreaWidget->zoomRatio);
    mVertRuler->setRulerZoom(drawAreaWidget->zoomRatio);
    mVertRuler->setRulerUnit(1/drawAreaWidget->zoomRatio);
    update();
}
void Scroll::keyPressEvent(QKeyEvent *event){
    qDebug()<<"Scroll "<<__FUNCTION__<<event->text()<<event->key();

        if (drawAreaWidget->needKeyboard){
            drawAreaWidget->keyPressEvent(event);
            return;
        }

    //drawAreaWidget->keyPressEvent(event);
    event->ignore();//propagated to mdi!!
}

void Scroll::keyReleaseEvent(QKeyEvent *event){
    //qDebug()<<"Scroll "<<__FUNCTION__<<event->text()<<event->key();
    //drawAreaWidget->keyReleaseEvent(event);
    event->ignore();
}
void  Scroll::contextMenuEvent( QContextMenuEvent * event ){
    //qDebug()<<"Scroll "<<__FUNCTION__;
    drawAreaWidget->contextMenuEvent(event);
}
