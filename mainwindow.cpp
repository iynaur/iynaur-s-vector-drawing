#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawareawidget.h"
#include "scroll.h"
#include <QScrollBar>
#include <QMdiSubWindow>
#include <QString>
//#include <QAbstractSocket>

//QString EnumSocketTypeToString(int value)
//{
//QMetaObject obj = ActionType::staticMetaObject;
//QMetaEnum en = obj.enumerator(0);
//return QLatin1String(en.valueToKey(value));
//}
int DrawAreaWidget::numOfFiles = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("DRAW");
    //ui->actionCloseCurve->setIcon(QIcon(tr(":/image/pencapstyle.png")));
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    setAcceptDrops(true);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow* )),
            this, SLOT(onSubWindowActivated(QMdiSubWindow* )));


    currentCategory = PickCategory;
    setWindowState(Qt::WindowMaximized);
    menuUndoTo=new QMenu;
    menuRedoTo=new QMenu;

    undoButton=new QToolButton;
    ui->toolBar_2->insertWidget(ui->actionCut,static_cast<QWidget*>(undoButton));
    undoButton->setMenu(menuUndoTo);
    undoButton->setPopupMode(QToolButton::MenuButtonPopup);
    undoButton->setDefaultAction(ui->actionUndo);

    redoButton=new QToolButton;
    ui->toolBar_2->insertWidget(ui->actionCut,static_cast<QWidget*>(redoButton));
    redoButton->setMenu(menuRedoTo);
    redoButton->setPopupMode(QToolButton::MenuButtonPopup);
    redoButton->setDefaultAction(ui->actionRedo);

    on_actionNew_triggered();
    copyShapes=new QList<shared_ptr<GeneralShape>>;//mem leak
    connect(menuRedoTo, SIGNAL(triggered(QAction*)), this, SLOT(RedoTo(QAction*)));
    connect(menuUndoTo, SIGNAL(triggered(QAction*)), this, SLOT(UndoTo(QAction*)));





}


void  MainWindow::onSubWindowActivated(QMdiSubWindow *window){
    //qDebug()<<activeMdiChild();
    if (window!=0)
    {
        if (le!=window){
            le=window;
            scrollArea=(Scroll *)(le->widget());
            scrollArea->drawAreaWidget->setCategory(currentCategory);
            updateToolBar();
        }
        updateToolBar();





    }
    else{//window==0
        if (mdiArea->subWindowList().size()==0){
        le=0;
        scrollArea=0;
        updateToolBar();
        }
    }
}
void MainWindow::updateToolBar(){
    if (le==0) {
        menuUndoTo->setEnabled(false);
        menuRedoTo->setEnabled(false);

        ui->actionPaste->setEnabled(false);
        ui->actionCircle->setEnabled(false);
        ui->actionCloseCurve->setEnabled(false);
        ui->actionCurve->setEnabled(false);
        ui->actionEllipse->setEnabled(false);
        ui->actionPalm->setEnabled(false);
        ui->actionPick->setEnabled(false);
        ui->actionPolygon->setEnabled(false);
        ui->actionPolyline->setEnabled(false);
        ui->actionRect->setEnabled(false);
        ui->actionText->setEnabled(false);

        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionMoveToBottom->setEnabled(false);
        ui->actionMoveToTop->setEnabled(false);
        ui->actionDivide->setEnabled(false);
        ui->actionDivideToEnd->setEnabled(false);
        ui->actionCombination->setEnabled(false);
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionZoomIn->setEnabled(false);
        ui->actionZoomOut->setEnabled(false);
        ui->actionZoomOne->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSaveAs->setEnabled(false);
        ui->actionAdd->setEnabled(false);
        ui->actionExpand->setEnabled(false);
        ui->actionFixcanvas->setEnabled(false);


    }
    else{//存在活动窗口
        if (scrollArea->drawAreaWidget->changed==false){
            le->setWindowTitle(scrollArea->drawAreaWidget->filename);
        }
        else{
            le->setWindowTitle(scrollArea->drawAreaWidget->filename+" *");
        }

        ui->actionPaste->setEnabled(!copyShapes->isEmpty());

        ui->actionCircle->setEnabled(true);
        ui->actionCloseCurve->setEnabled(true);
        ui->actionCurve->setEnabled(true);
        ui->actionEllipse->setEnabled(true);
        ui->actionPalm->setEnabled(true);
        ui->actionPick->setEnabled(true);
        ui->actionPolygon->setEnabled(true);
        ui->actionPolyline->setEnabled(true);
        ui->actionRect->setEnabled(true);
        ui->actionText->setEnabled(true);

        ui->actionZoomIn->setEnabled(true);
        ui->actionZoomOut->setEnabled(true);
        ui->actionZoomOne->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionSaveAs->setEnabled(true);
        ui->actionAdd->setEnabled(true);
        ui->actionExpand->setEnabled(true);
        ui->actionFixcanvas->setEnabled(true);

        if (scrollArea->drawAreaWidget->pickedShapes.size()>0){

            if (scrollArea->drawAreaWidget->pickedShapes.size()>1){
                ui->actionCombination->setEnabled(true);
                ui->actionDivide->setEnabled(false);
                ui->actionDivideToEnd->setEnabled(false);
            } else{//=1
                ui->actionCombination->setEnabled(false);
                if(scrollArea->drawAreaWidget->pickedShapes.at(0)->name()=="Combo"){
                    ui->actionDivide->setEnabled(true);
                    ui->actionDivideToEnd->setEnabled(true);
                }else{
                    ui->actionDivide->setEnabled(false);
                    ui->actionDivideToEnd->setEnabled(false);
                }
            }
            ui->actionCopy->setEnabled(true);
            ui->actionCut->setEnabled(true);
            ui->actionMoveToBottom->setEnabled(true);
            ui->actionMoveToTop->setEnabled(true);

        }
        else{
            ui->actionCombination->setEnabled(false);

                ui->actionDivide->setEnabled(false);
                ui->actionDivideToEnd->setEnabled(false);


            ui->actionCopy->setEnabled(false);
            ui->actionCut->setEnabled(false);
            ui->actionMoveToBottom->setEnabled(false);
            ui->actionMoveToTop->setEnabled(false);
        }
        if (scrollArea->drawAreaWidget->undoList.index()==0) ui->actionUndo->setEnabled(false);
        else        ui->actionUndo->setEnabled(true);

        if (scrollArea->drawAreaWidget->undoList.index()==scrollArea->drawAreaWidget->undoList.count()) ui->actionRedo->setEnabled(false);
        else         ui->actionRedo->setEnabled(true);

    ui->actionCircle->setChecked(false);
    ui->actionCloseCurve->setChecked(false);
    ui->actionCurve->setChecked(false);
    ui->actionEllipse->setChecked(false);
    ui->actionPalm->setChecked(false);
    ui->actionPick->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionPolyline->setChecked(false);
    ui->actionRect->setChecked(false);
    ui->actionText->setChecked(false);


    switch (currentCategory){
    case CircleCategory: {
            ui->actionCircle->setChecked(true);
            break;
        }
    case CloseCurveCategory: {
            ui->actionCloseCurve->setChecked(true);
            break;
        }
    case CurveCategory: {
            ui->actionCurve->setChecked(true);
            break;
        }
    case EllipseCategory: {
            ui->actionEllipse->setChecked(true);
            break;
        }
    case PalmCategory: {
            ui->actionPalm->setChecked(true);
            break;
        }
    case PickCategory: {
            ui->actionPick->setChecked(true);
            break;
        }
    case PolygonCategory: {
            ui->actionPolygon->setChecked(true);
            break;
        }
    case PolylineCategory: {
            ui->actionPolyline->setChecked(true);
            break;
        }
    case RectCategory: {
            ui->actionRect->setChecked(true);
            break;
        }
    case TextCategory: {
            ui->actionText->setChecked(true);
            break;
        }
    }
    if (scrollArea->drawAreaWidget->undoList.count()>0) {
        while(!undoList.isEmpty()){
            QAction *qaction=undoList.at(0);
            menuUndoTo->removeAction(qaction);
            delete qaction;
            undoList.removeFirst();
        }
        //menuUndoTo->clear();
        menuUndoTo->setEnabled(false);
        for (int i=scrollArea->drawAreaWidget->undoList.index()-1;  i>=0;  i--){
            QAction* action =(new QAction(dynamic_cast<AbstractAction*>(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoList.command(i)))->name()+" "
                                         +(dynamic_cast<AbstractAction*>(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoList.command(i)))->shapes.size()>1?
                                             "multipal shapes":
                                             dynamic_cast<AbstractAction*>(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoList.command(i)))->shapes.at(0)->name())));
                    action->setData(i+1);

                    menuUndoTo->addAction(action);
                    undoList.append(action);
                    //qDebug()<<"addaction";
                    menuUndoTo->setEnabled(true);


        }
        //connect(menuUndoTo, SIGNAL(triggered(QAction*)), this, SLOT(UndoTo(QAction*)));//strange error connect several times!!!!
        //

        while(!redoList.isEmpty()){
            QAction *qaction=redoList.at(0);
            menuRedoTo->removeAction(qaction);
            delete qaction;
            redoList.removeFirst();
        }
        //menuRedoTo->clear();
         menuRedoTo->setEnabled(false);
        for (int i=scrollArea->drawAreaWidget->undoList.index();  i<scrollArea->drawAreaWidget->undoList.count();  i++){
            QAction* action =(new QAction(dynamic_cast<AbstractAction*>(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoList.command(i)))->name()+" "
                                         +(dynamic_cast<AbstractAction*>(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoList.command(i)))->shapes.size()>1?
                                             "multipal shapes":
                                             dynamic_cast<AbstractAction*>(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoList.command(i)))->shapes.at(0)->name())));
                    action->setData(i);

                    menuRedoTo->addAction(action);
                    redoList.append(action);
                    //qDebug()<<"addaction";
                     menuRedoTo->setEnabled(true);


        }

    }
    else{
        menuUndoTo->setEnabled(false);
        menuRedoTo->setEnabled(false);
    }
    }

}


void MainWindow::UndoTo(QAction* action){
     int index = action->data().toInt();

    while (scrollArea->drawAreaWidget->undoList.index()>=index){
        scrollArea->drawAreaWidget->undo();
    }
}

void MainWindow::RedoTo(QAction* action){
    int index = action->data().toInt();//

    while (scrollArea->drawAreaWidget->undoList.index()<=index){
        scrollArea->drawAreaWidget->redo();
    }
}



void MainWindow::resizeEvent(QResizeEvent *event){
//    scrollArea->drawAreaWidget->windowwidth=scrollArea->width();
//    scrollArea->drawAreaWidget->windowheight=scrollArea->height();
}

MainWindow::~MainWindow()
{

    delete copyShapes;
    //menuUndoTo->clear();
    //menuRedoTo->clear();
    while(!redoList.isEmpty()){
        QAction *qaction=redoList.at(0);
        menuRedoTo->removeAction(qaction);
        delete qaction;
        redoList.removeFirst();
    }
    while(!undoList.isEmpty()){
        QAction *qaction=undoList.at(0);
        menuUndoTo->removeAction(qaction);
        delete qaction;
        undoList.removeFirst();
    }
    delete mdiArea;
    delete undoButton;
    delete redoButton;
    delete menuUndoTo;
    delete menuRedoTo;
    delete ui;
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)//拖进事件

{
    if(event->mimeData()->hasFormat("text/uri-list")){
        event->setDropAction(Qt::IgnoreAction);
        event->acceptProposedAction();
    }
}
void MainWindow::dropEvent(QDropEvent *event)//放下事件
{
    qDebug()<<event->mimeData()->urls();
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
            on_actionNew_triggered();
            scrollArea->drawAreaWidget->openasadd=false;
            handleMessage(fileName);
        }
    }
}




void MainWindow::handleMessage(QString message){
    //QString openfile=message;
    scrollArea->drawAreaWidget->openfile(message);
}

void MainWindow::keyPressEvent(QKeyEvent *event){//上下左右键被scroll bar拦截了，到不了这里！！只有WSAD
    //qDebug()<<event->key();
    switch (event->key())
    case Qt::Key_Delete:
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:{

        scrollArea->drawAreaWidget->keyPressEvent(event);
        break;
    }

}
void MainWindow::keyReleaseEvent(QKeyEvent *event){//上下左右键被scroll bar拦截了，到不了这里！！只有WSAD
    //qDebug()<<event->key();
    switch (event->key())
    case Qt::Key_Delete:
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:{

        scrollArea->drawAreaWidget->keyReleaseEvent(event);
        break;
    }

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
//void MainWindow::on_actionRotate_triggered(){
//    scrollArea->drawAreaWidget->rotate();
//}
void  MainWindow::on_actionMoveToTop_triggered(){
    scrollArea->drawAreaWidget->moveToTop();
}

void  MainWindow::on_actionMoveToBottom_triggered(){
    scrollArea->drawAreaWidget->moveToBottom();
}

void MainWindow::on_actionCut_triggered()
{
    QList<shared_ptr<GeneralShape>>* tmp=new QList<shared_ptr<GeneralShape>>;
    *tmp=scrollArea->drawAreaWidget->cut();
    if((*tmp).size()>0){
        copyShapes=tmp;
    }
    updateToolBar();
}

void  MainWindow::on_actionCopy_triggered(){
    QList<shared_ptr<GeneralShape>>* tmp=new QList<shared_ptr<GeneralShape>>;
    *tmp=scrollArea->drawAreaWidget->copy();
    //qDebug()<<"---";
    if(tmp->size()>0){
        copyShapes=tmp;
    }
    updateToolBar();
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

void  MainWindow::on_actionNew_triggered(){


    scrollArea = new Scroll;

    le= mdiArea->addSubWindow(scrollArea);

    le->resize(1000,600);
    le->setGeometry(30*DrawAreaWidget::numOfFiles,30*DrawAreaWidget::numOfFiles,1000,600);
    //le->setWindowState(Qt::WindowMaximized);
    le->show();
    connect(scrollArea->drawAreaWidget, SIGNAL(categoryChanged()),
            this, SLOT(updateToolBar()));


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

void MainWindow::on_actionSaveAs_triggered()
{
    scrollArea->drawAreaWidget->saveAs();
}
