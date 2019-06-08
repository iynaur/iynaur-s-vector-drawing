#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui=nullptr;//to test dump
    ui->setupUi(this);

    m_allBuilders = getAllBuilders();

    for (auto it = m_allBuilders.begin(); it != m_allBuilders.end(); it++) {
        QAction* act = new QAction(it->first, 0);
        act->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral("image/")+ it->first + QStringLiteral(".png"), QSize(), QIcon::Normal, QIcon::Off);
        act->setIcon(icon1);
        connect(act, &QAction::triggered, this, &MainWindow::onAutoAction);
        ui->toolBar_1->addAction(act);
        ui->menuDraw->addAction(act);
    }
    //setIconSize(QSize(20, 20));
    le=0;
    setWindowTitle(tr("DRAW"));
    //ui->actionCloseCurve->setIcon(QIcon(tr("./image/pencapstyle.png")));
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    setAcceptDrops(true);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow* )),
            this, SLOT(onSubWindowActivated(QMdiSubWindow* )));


    currentCategory = PickCategory;
    settings=new QSettings ("draw.ini",QSettings::IniFormat);
    qDebug() << settings->fileName();
    readSettings();
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
    sld=new QSlider(this);
    sld->setOrientation(Qt::Horizontal);
    sld->setRange(-12,12);//对数坐标
    sld->setFixedWidth(100);
    ui->statusBar->addPermanentWidget(sld);
    lb=new QLabel(this);
    lb->setFixedWidth(40);
    ui->statusBar->addPermanentWidget(lb);
    zoom(0);
    connect(sld,SIGNAL(valueChanged(int)),this,SLOT(zoom(int)));
}
void MainWindow::onAutoAction() {
    QObject* obj = sender();
    auto act = dynamic_cast<QAction*>(obj);
    if (act) {
        for (auto it = m_allBuilders.begin(); it != m_allBuilders.end(); it++) {
            if (it->first == act->text()) {
                currentCategory = it->second;
                scrollArea->drawAreaWidget->setCategory(currentCategory);
                break;
            }
        }
    }
    for (auto a : ui->toolBar_1->actions()) {
        a->setChecked(false);
    }
    act->setChecked(true);
}

void  MainWindow::onSubWindowActivated(QMdiSubWindow *window){
    //qDebug()<<activeMdiChild();

    if (window!=0)
    {
        settings->setValue(tr("subWindowIsMaximized"),window->isMaximized());
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
        setAllShapeBuildersEnabled(false);
        ui->actionPalm->setEnabled(false);
        ui->actionPick->setEnabled(false);

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
        ui->actionPrint->setEnabled(false);
        ui->actionPrintPreview->setEnabled(false);


    }
    else{//存在活动窗口
        if (scrollArea->drawAreaWidget->saveIndex==scrollArea->drawAreaWidget->undoStack.index()){
            le->setWindowTitle(scrollArea->drawAreaWidget->filename);
        }
        else{
            le->setWindowTitle(scrollArea->drawAreaWidget->filename+" *");
        }

        ui->actionPaste->setEnabled(!copyShapes->isEmpty());

        setAllShapeBuildersEnabled(true);
        ui->actionPalm->setEnabled(true);
        ui->actionPick->setEnabled(true);

        ui->actionZoomIn->setEnabled(true);
        ui->actionZoomOut->setEnabled(true);
        ui->actionZoomOne->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionSaveAs->setEnabled(true);
        ui->actionAdd->setEnabled(true);
        ui->actionExpand->setEnabled(true);
        ui->actionFixcanvas->setEnabled(true);
        ui->actionPrint->setEnabled(true);
        ui->actionPrintPreview->setEnabled(true);

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
        if (scrollArea->drawAreaWidget->undoStack.index()==0) ui->actionUndo->setEnabled(false);
        else        ui->actionUndo->setEnabled(true);

        if (scrollArea->drawAreaWidget->undoStack.index()==scrollArea->drawAreaWidget->undoStack.count()) ui->actionRedo->setEnabled(false);
        else         ui->actionRedo->setEnabled(true);

        //setAllShapeBuildersChecked(false);
        ui->actionPalm->setChecked(false);
        ui->actionPick->setChecked(false);


        switch (currentCategory){
        case PalmCategory: {
                ui->actionPalm->setChecked(true);
                setAllShapeBuildersChecked(false);
                break;
            }
        case PickCategory: {
                ui->actionPick->setChecked(true);
                setAllShapeBuildersChecked(false);
                break;
            }
        }
        if (scrollArea->drawAreaWidget->undoStack.count()>0) {
            while(!undoList.isEmpty()){
                QAction *qaction=undoList.at(0);
                menuUndoTo->removeAction(qaction);
                delete qaction;
                undoList.removeFirst();
            }
            //menuUndoTo->clear();
            menuUndoTo->setEnabled(false);
            for (int i=scrollArea->drawAreaWidget->undoStack.index()-1;  i>=0;  i--){
                QString actionText=(const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoStack.command(i)))->text();

                QAction* action =new QAction(actionText,this);
                        action->setData(i+1);

                        menuUndoTo->addAction(action);
                        undoList.append(action);
                        //qDebug()<<"addaction";
                        menuUndoTo->setEnabled(true);
            }

            while(!redoList.isEmpty()){
                QAction *qaction=redoList.at(0);
                menuRedoTo->removeAction(qaction);
                delete qaction;
                redoList.removeFirst();
            }
            //menuRedoTo->clear();
            menuRedoTo->setEnabled(false);
            for (int i=scrollArea->drawAreaWidget->undoStack.index();  i<scrollArea->drawAreaWidget->undoStack.count();  i++){
                QAction* action =new QAction((const_cast<QUndoCommand*>(scrollArea->drawAreaWidget->undoStack.command(i)))->text()
                                             ,this);
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

    while (scrollArea->drawAreaWidget->undoStack.index()>=index){
        scrollArea->drawAreaWidget->undo();
    }
}

void MainWindow::RedoTo(QAction* action){
    int index = action->data().toInt();//

    while (scrollArea->drawAreaWidget->undoStack.index()<=index){
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
    //qDebug()<<event->mimeData()->urls();
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
    qDebug()<<"MainWindow "<<__FUNCTION__<<event->text()<<event->key();

        if (event->matches(QKeySequence::Cut))
        {
            on_actionCut_triggered();
            return;
        }
        if (event->matches(QKeySequence::Copy))
        {
            on_actionCopy_triggered();
            return;
        }
        if (event->matches(QKeySequence::Paste))
        {
            on_actionPaste_triggered();
            return;
        }


    if (scrollArea){
        scrollArea->drawAreaWidget->keyPressEvent(event);


    }
    QMainWindow::keyPressEvent(event);
}
void MainWindow::keyReleaseEvent(QKeyEvent *event){//上下左右键被scroll bar拦截了，到不了这里！！只有WSAD
    //qDebug()<<"MainWindow "<<__FUNCTION__<<event->text()<<event->key();
    if (scrollArea){
        scrollArea->drawAreaWidget->keyReleaseEvent(event);
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->setAccepted(false);
    foreach(QMdiSubWindow * swin,mdiArea->subWindowList()){
        settings->setValue(tr("subWindowIsMaximized"),swin->isMaximized());

        if (!swin->close()) {
            return;
        }
    }
    settings->setValue(tr("mainWindowIsMaximized"),this->isMaximized());
    settings->setValue(tr("pos"),this->pos());
    settings->setValue(tr("size"),this->size());
    settings->setValue(tr("toolBar_0_Area"),toolBarArea(ui->toolBar_0));
    settings->setValue(tr("toolBar_0_Visible"),ui->toolBar_0->isVisible());
    settings->setValue(tr("toolBar_1_Area"),toolBarArea(ui->toolBar_1));
    settings->setValue(tr("toolBar_1_Visible"),ui->toolBar_1->isVisible());
    settings->setValue(tr("toolBar_2_Area"),toolBarArea(ui->toolBar_2));
    settings->setValue(tr("toolBar_2_Visible"),ui->toolBar_2->isVisible());
    settings->sync();
    event->accept();
}
void MainWindow::setAllShapeBuildersEnabled(bool enable)
{
    for (auto a : ui->toolBar_1->actions()) {
        a->setEnabled(enable);
    }
}
void MainWindow::setAllShapeBuildersChecked(bool enable)
{
    for (auto a : ui->toolBar_1->actions()) {
        a->setChecked(enable);
    }
}
void MainWindow::on_actionPick_triggered(){
    currentCategory=PickCategory;
    scrollArea->drawAreaWidget->setCategory(PickCategory);
}
void MainWindow::on_actionFixcanvas_triggered(){
    scrollArea->drawAreaWidget->fitcanvas();
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
    scrollArea->drawAreaWidget->zoom(0.5);
}
void MainWindow::on_actionZoomOut_triggered(){
//    scrollArea->drawAreaWidget->zoomRatio=scrollArea->drawAreaWidget->zoomRatio*1.6;
    scrollArea->drawAreaWidget->zoom(2);
}
void  MainWindow::on_actionZoomOne_triggered(){
    scrollArea->drawAreaWidget->zoomone();

}
void MainWindow::zoom(int z){
    if (scrollArea){
        scrollArea->drawAreaWidget->zoom(pow(2.0,z/4.0)/scrollArea->drawAreaWidget->zoomRatio);
        lb->setText(QString("%1").arg(int(pow(2.0,z/4.0)*100))+"%");
    }
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
    //qDebug()<<"on_actionCut_triggered";
    QList<shared_ptr<GeneralShape>>* tmp=new QList<shared_ptr<GeneralShape>>;
    *tmp=scrollArea->drawAreaWidget->cut();
    if((*tmp).size()>0){
        copyShapes=tmp;
    }
    updateToolBar();
}

void  MainWindow::on_actionCopy_triggered(){
    //qDebug()<<"on_actionCopy_triggered";
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
    scrollArea->setParent(this);
    scrollArea->drawAreaWidget->setCategory(currentCategory);
    le= mdiArea->addSubWindow(scrollArea);

    le->resize(1000,600);
    le->setGeometry(30*DrawAreaWidget::numOfFiles,30*DrawAreaWidget::numOfFiles,1000,600);
    //le->setWindowState(Qt::WindowMaximized);
    bool state = settings->value("subWindowIsMaximized",true).toBool();
    if (state){
        le->setWindowState(Qt::WindowMaximized);
    }
    le->show();
    connect(scrollArea->drawAreaWidget, SIGNAL(categoryChanged()),
            this, SLOT(updateToolBar()));
    connect(scrollArea->drawAreaWidget,SIGNAL(mouseMoved(QMouseEvent*,QPointF)),this , SLOT(updateStatusBar(QMouseEvent*,QPointF)));
    connect(scrollArea->drawAreaWidget,SIGNAL(mouseLeave()),this , SLOT(clearStatusBar()));

}
void MainWindow::clearStatusBar(){
    ui->statusBar->showMessage("");
}

void MainWindow::updateStatusBar(QMouseEvent*,QPointF p){
    ui->statusBar->showMessage("X:"+QString("%1").arg(p.x(),4,'f',4)+" Y:"+QString("%1").arg(p.y(),4,'f',4));

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

void MainWindow::on_actionPrint_triggered()
{
    scrollArea->drawAreaWidget->print();
}

void MainWindow::on_actionPrintPreview_triggered()
{
    scrollArea->drawAreaWidget->printPreview();
}
void MainWindow::on_actionClearSettings_triggered()
{
    //clearSettings=true;
    //QSettings settings("MySoft", "draw");
    settings->clear();
    settings->sync();
    readSettings();
}
void MainWindow::readSettings(){
    //QSettings settings("MySoft", "draw");
    ui->toolBar_0->setVisible(settings->value("toolBar_0_Visible",true).toBool());
    addToolBar(Qt::ToolBarArea(settings->value("toolBar_0_Area",Qt::TopToolBarArea).toInt()),ui->toolBar_0);
    ui->toolBar_1->setVisible(settings->value("toolBar_1_Visible",true).toBool());
    addToolBar(Qt::ToolBarArea(settings->value("toolBar_1_Area",Qt::TopToolBarArea).toInt()),ui->toolBar_1);
    ui->toolBar_2->setVisible(settings->value("toolBar_2_Visible",true).toBool());
    addToolBar(Qt::ToolBarArea(settings->value("toolBar_2_Area",Qt::TopToolBarArea).toInt()),ui->toolBar_2);
    if (settings->value("mainWindowIsMaximized",true).toBool()){
        setWindowState(Qt::WindowMaximized);
    }else{
        move(settings->value("pos").toPoint());
        resize(settings->value("size").toSize());
    }
    if (le!=0){
        if (settings->value("subWindowIsMaximized",true).toBool()){
        le->setWindowState(Qt::WindowMaximized);
        }else{
            le->setWindowState(Qt::WindowNoState);
        }
    }
}
