#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawareawidget.h"
#include "scroll.h"
#include <QScrollBar>
#include <QMdiSubWindow>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //ui->actionCloseCurve->setIcon(QIcon(tr(":/image/pencapstyle.png")));
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow* )),
            this, SLOT(onSubWindowActivated(QMdiSubWindow* )));


    currentCategory = PickCategory;
    setWindowState(Qt::WindowMaximized);

    on_actionNew_triggered();
    //copyShapes=new QList<GeneralShape *>;


    //ui->setupUi(this);
}

void  MainWindow::onSubWindowActivated(QMdiSubWindow *window){
    //qDebug()<<activeMdiChild();
    if (window!=0)
    {
        le=window;
       scrollArea=(Scroll *)(le->widget());
       scrollArea->drawAreaWidget->setCategory(currentCategory);



           //scrollArea=child;
           //qDebug()<<"shuffled";

    }
    else{
        le=0;
        scrollArea=0;
    }
}


void MainWindow::resizeEvent(QResizeEvent *event){
//    scrollArea->drawAreaWidget->setFixedSize(width(),height());
//    scrollArea->drawAreaWidget->expand();
    scrollArea->drawAreaWidget->windowwidth=scrollArea->width();
    scrollArea->drawAreaWidget->windowheight=scrollArea->height();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleMessage(QString message){
    //QString openfile=message;
    scrollArea->drawAreaWidget->openfile(message);
}

void MainWindow::keyPressEvent(QKeyEvent *event){//上下左右键被scroll bar拦截了，到不了这里！！
    //qDebug()<<event->key();

        scrollArea->drawAreaWidget->keyPressEvent(event);

}


//Scroll *MainWindow::activeMdiChild()
//{
//    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
//        return dynamic_cast<Scroll*>(activeSubWindow->widget());
//    return 0;
//}

void MainWindow::closeEvent(QCloseEvent *event)
{
   foreach(QMdiSubWindow * swin,mdiArea->subWindowList()){
       //le=mdiArea->activeSubWindow();
      scrollArea=(Scroll *)(swin->widget());
       scrollArea->closeEvent(event);
   }
}
void MainWindow::on_actionCurve_triggered(){
    currentCategory=CurveCategory;
    scrollArea->drawAreaWidget->setCategory(CurveCategory);
}
void MainWindow::on_actionCloseCurve_triggered(){
    currentCategory=CloseCurveCategory;
    scrollArea->drawAreaWidget->setCategory(CloseCurveCategory);
}
void MainWindow::on_actionPolyline_triggered(){
    currentCategory=PolylineCategory;
    scrollArea->drawAreaWidget->setCategory(PolylineCategory);
}
void MainWindow::on_actionPolygon_triggered(){
    currentCategory=PolygonCategory;
    scrollArea->drawAreaWidget->setCategory(PolygonCategory);
}
void MainWindow::on_actionPick_triggered(){
    currentCategory=PickCategory;
    scrollArea->drawAreaWidget->setCategory(PickCategory);
}
void MainWindow::on_actionCircle_triggered(){
    currentCategory=CircleCategory;
    scrollArea->drawAreaWidget->setCategory(CircleCategory);
}
void MainWindow::on_actionFixcanvas_triggered(){
    scrollArea->drawAreaWidget->fitcanvas();
}
void MainWindow::on_actionRect_triggered(){
    currentCategory=RectCategory;
    scrollArea->drawAreaWidget->setCategory(RectCategory);
}
void MainWindow::on_actionEllipse_triggered(){
    currentCategory=EllipseCategory;
    scrollArea->drawAreaWidget->setCategory(EllipseCategory);
}
void MainWindow::on_actionText_triggered(){
    currentCategory=TextCategory;
    scrollArea->drawAreaWidget->setCategory(TextCategory);
}
void MainWindow::on_actionSave_triggered(){

    scrollArea->drawAreaWidget->save();
}
void MainWindow::on_actionOpen_triggered(){
    if (scrollArea==0){
        on_actionNew_triggered();
    }
    scrollArea->drawAreaWidget->opennew();
}
void MainWindow::on_actionAdd_triggered(){
    scrollArea->drawAreaWidget->addfile();
}
void MainWindow::on_actionZoomIn_triggered(){
    //scrollArea->drawAreaWidget->zoomRatio=scrollArea->drawAreaWidget->zoomRatio*0.625;
    scrollArea->drawAreaWidget->zoom(0.625);
}
void MainWindow::on_actionZoomOut_triggered(){
//    scrollArea->drawAreaWidget->zoomRatio=scrollArea->drawAreaWidget->zoomRatio*1.6;
    scrollArea->drawAreaWidget->zoom(1.6);
}
void  MainWindow::on_actionZoomOne_triggered(){
    scrollArea->drawAreaWidget->zoomone();

}

void  MainWindow::on_actionPalm_triggered(){
    currentCategory=PalmCategory;
    scrollArea->drawAreaWidget->setCategory(PalmCategory);
}
void MainWindow::on_actionRotate_triggered(){
    scrollArea->drawAreaWidget->rotate();
}
void  MainWindow::on_actionMoveToTop_triggered(){
    scrollArea->drawAreaWidget->moveToTop();
}

void  MainWindow::on_actionMoveToBottom_triggered(){
    scrollArea->drawAreaWidget->moveToBottom();
}
void  MainWindow::on_actionChangeToClose_triggered(){
    scrollArea->drawAreaWidget->changeToClose();
}

void MainWindow::on_actionCut_triggered()
{
    QList<GeneralShape *>* tmp=new QList<GeneralShape *>;
    *tmp=scrollArea->drawAreaWidget->cut();
    if((*tmp).size()>0){
        copyShapes=tmp;
    }
}

void  MainWindow::on_actionCopy_triggered(){
    QList<GeneralShape *>* tmp=new QList<GeneralShape *>;
    *tmp=scrollArea->drawAreaWidget->copy();
    //qDebug()<<"---";
    if(tmp->size()>0){
        copyShapes=tmp;
    }
    //*copyShapes=scrollArea->drawAreaWidget->copy();
}
void  MainWindow::on_actionPaste_triggered(){
    scrollArea->drawAreaWidget->paste(copyShapes);
}
void  MainWindow::on_actionCombination_triggered(){
    scrollArea->drawAreaWidget->combination();
}
void  MainWindow::on_actionDivide_triggered(){
    scrollArea->drawAreaWidget->divide();
}
 void  MainWindow::on_actionDivideToEnd_triggered(){
     scrollArea->drawAreaWidget->divideToEnd();
 }
 void  MainWindow::on_actionTest_triggered(){
     //scrollArea->drawAreaWidget->test();
 }
void  MainWindow::on_actionNew_triggered(){


    scrollArea = new Scroll;

    le= mdiArea->addSubWindow(scrollArea);

    le->resize(700,600);
    le->setWindowState(Qt::WindowMaximized);
    le->show();


}

//void  MainWindow::on_actionOpenOldFileFormat_triggered(){
//    scrollArea->drawAreaWidget->openOldFileFormat();
//}

void MainWindow::on_actionExpand_triggered()
{
    scrollArea->drawAreaWidget->expand();
}


void MainWindow::on_actionUndo_triggered()
{
    scrollArea->drawAreaWidget->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    scrollArea->drawAreaWidget->redo();
}
