#include "drawareawidget.h"
#include "gettextdialog.h"

#include <QDebug>
#include <algorithm>
#include <QTextStream>
#include <QApplication>
//#include <string>
//#include <QtWidgets>//this is it , cause a lot of "does not name a type" error
#include <sstream>
//#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QtXml>
#include <QColorDialog>
#include <QFontDialog>
#include <QMenu>
using namespace std;
DrawAreaWidget::DrawAreaWidget(QWidget *parent) :
    QWidget(parent)
{
    actionMoveToTop     = new QAction(tr("MoveToTop"), this);
    actionMoveToBottom     = new QAction(tr("MoveToBottom"), this);
    actionSetBrush= new QAction(tr("SetBrush"), this);
    actionSetBrush->setIcon(QIcon(QString::fromUtf8(":/image/pallet.png")));
    connect(actionMoveToTop, SIGNAL(triggered()), this, SLOT(moveToTop()));
    connect(actionMoveToBottom, SIGNAL(triggered()), this, SLOT(moveToBottom()));
    connect(actionSetBrush, SIGNAL(triggered()), this, SLOT(setBrush()));
    numOfFiles++;
    openasadd=false;
    pickRect=0;
    //root=0;
    init();
}
DrawAreaWidget::~DrawAreaWidget(){

//    for (int i=1;i<=actionList.size();i++){

//         //delete actionList.at(i-1);

//    }

    delete  actionMoveToTop;
    delete actionMoveToBottom;
    delete actionSetBrush;
    //delete pickRect;
}

void DrawAreaWidget::init()
{
    currentShape = NULL;
    pickedShapes.clear();
    //delete pickRect;
    pickRect=NULL;
    currentCategory = PickCategory;
    isLeftButtonPressed = false;
    shapes.clear();
    dx=dy=0;
    changed=false;

    zoomRatio=1;
    previouszoomRatio=1;
    backcolor=Qt::white;
    currentMouseHanded=None;
    //actionList.clear();
    //actionindex=0;
    saveIndex=0;

    filename="New Pic "+QString::number(numOfFiles, 10);

//     canredo=false;
//     canundo=false;
//     cancut=false;
//     cancopy=false;
//     //canpaste;
//     cantop=false;
//     canbottom=false;
//     cancombo=false;
//     candivide=false;
     emit categoryChanged();
}

void DrawAreaWidget::combination(){
    if (pickedShapes.size()<=1) return;
    shared_ptr<Combo> tmp=shared_ptr<Combo>(new Combo);//mem leak;
    CombineAction* action=new CombineAction();
    foreach(shared_ptr<GeneralShape> sp,pickedShapes){
        //shared_ptr<GeneralShape> sp2=sp;//->copyPaste();
        tmp->shapes.append(sp);
        action->shapes.append(sp);
        action->indexOfShapes.append(shapes.indexOf(sp));
        //del(sp);
        //// sp;
    }
    action->com=tmp;

    //actionList.append(static_cast<AbstractAction*>(action));
    tmp->updateRange();
    pickedShapes.clear();
    pickedShapes.append(tmp);
    //shapes.append(tmp);
    //update();
    addaction(static_cast<AbstractAction*>(action));
    //emit categoryChanged();

}
void DrawAreaWidget::divide(){
    if (pickedShapes.size()!=1 || pickedShapes.at(0)->name()!="Combo") return;
    shared_ptr<GeneralShape> tmp=pickedShapes.at(0);
    pickedShapes.clear();
    divide(tmp);
    //del(tmp);
    update();
    emit categoryChanged();
}
void DrawAreaWidget::divide(shared_ptr<GeneralShape> shape){
    shared_ptr<Combo> tmp=dynamic_pointer_cast<Combo >(shape);
    DivideAction* action=new DivideAction();
    action->com=tmp;
    action->indexOfCom=shapes.indexOf(shape);
    foreach(shared_ptr<GeneralShape> sp,tmp->shapes){
        //shared_ptr<GeneralShape>sp2=sp->copyPaste();
        action->shapes.append(sp);
        //getOutOfCombo(sp,tmp);
    }
    pickedShapes.operator +=(action->shapes);
    addaction(static_cast<AbstractAction*>(action));

}

void DrawAreaWidget::divideToEnd(){
    if (pickedShapes.size()!=1 || pickedShapes.at(0)->name()!="Combo") return;
    shared_ptr<GeneralShape> tmp=pickedShapes.at(0);
    pickedShapes.clear();

    divideToEnd(tmp);
    update();
    emit categoryChanged();

}
void DrawAreaWidget::divideToEnd(shared_ptr<GeneralShape> shape){
    if (shape->name()!="Combo") return;
    pickedShapes.removeOne(shape);
    //Combo* tmp=dynamic_cast<Combo* >(shape);
    divide(shape);
    foreach(shared_ptr<GeneralShape> sp,dynamic_pointer_cast<Combo>(shape)->shapes){

        //getOutOfCombo(sp,tmp);
        divideToEnd(sp);

    }
    //del(shape);

}
//void DrawAreaWidget::getOutOfCombo(shared_ptr<GeneralShape> sp,shared_ptr<Combo> tmp){
//    QPointF spcentralPoint=sp->centralPoint();
//    sp->drag(-spcentralPoint);
//    sp->Rotationangle=sp->Rotationangle+tmp->Rotationangle;
//    //sp->drag(tmp->centralPoint());
//    sp->drag(rotated(spcentralPoint-tmp->centralPoint(),-tmp->Rotationangle/180*M_PI));//rotate done
//    //sp->drag(-tmp->centralPoint());
//    sp->zoom(tmp->getsx());
//    sp->drag(tmp->centralPoint());//scale done

//}

//void DrawAreaWidget::getIntoCombo(shared_ptr<GeneralShape> sp, shared_ptr<Combo> tmp){

//    QPointF spcentralPoint=sp->centralPoint();
//    sp->drag(-spcentralPoint);
//    sp->drag(rotated(spcentralPoint-tmp->centralPoint(),tmp->Rotationangle/180*M_PI));
//    sp->zoom(1/tmp->getsx());
//    sp->Rotationangle=sp->Rotationangle-tmp->Rotationangle;
//    sp->drag(tmp->centralPoint());
//}

void DrawAreaWidget::copyPaste(){

    if (pickedShapes.size()>0) {
        QList<shared_ptr<GeneralShape>> copiedshapes;
        foreach(shared_ptr<GeneralShape> sp,pickedShapes){
            shared_ptr<GeneralShape>tmp=sp->copyPaste();
            tmp->drag(QPointF(10,10));
            shapes.append(tmp);
            copiedshapes.append(tmp);
        }

        pickedShapes=copiedshapes;
        expand();update();
    }

}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::copy(){
    QList<shared_ptr<GeneralShape>> copiedshapes;
    if (pickedShapes.size()>0) {

        foreach(shared_ptr<GeneralShape> sp,pickedShapes){
            shared_ptr<GeneralShape>tmp=sp->copyPaste();

            copiedshapes.append(tmp);
        }


    }
    return copiedshapes;
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::cut(){
    QList<shared_ptr<GeneralShape>> copiedshapes;
    if (pickedShapes.size()>0) {
        DeleteAction *action=new DeleteAction();
        foreach(shared_ptr<GeneralShape> sp,pickedShapes){
            shared_ptr<GeneralShape>tmp=sp->copyPaste();
            action->shapes.append(sp);
            action->indexOfShapes.append(shapes.indexOf(sp));
            copiedshapes.append(tmp);

            //del(sp);
        }
        addaction(static_cast<AbstractAction*>(action));
        pickedShapes.clear();
        update();


    }
    return copiedshapes;
}
void DrawAreaWidget::paste(QList<shared_ptr<GeneralShape>>*copyShapes){

    if (copyShapes->size()>0) {
        QList<shared_ptr<GeneralShape>> copiedshapes;
        AbstractAction *action=new AddAction();
        for(int i=1;i<=copyShapes->size();i++){
            shared_ptr<GeneralShape>tmp=copyShapes->at(i-1)->copyPaste();
            tmp->drag(QPointF(10,10));
            //shapes.append(tmp);
            action->shapes.append(tmp);

            copiedshapes.append(tmp);
            copyShapes->removeAt(i-1);
            copyShapes->insert(i-1,tmp);
            //sp=tmp;
        }
        addaction(action);
//        foreach(shared_ptr<GeneralShape> sp,*copyShapes){
//            shared_ptr<GeneralShape>tmp=sp->copyPaste();
//            tmp->drag(QPointF(10,10));
//            shapes.append(tmp);
//            copiedshapes.append(tmp);
//            //sp=tmp;
//        }
        //copyShapes=&copiedshapes;
        pickedShapes=copiedshapes;
        expand();update();
    }
    emit categoryChanged();
}

//void DrawAreaWidget::rotate(){
////    if (pickedShapes.size()!=1) return;
////    pickedShapes.at(0)->setRotationangle( pickedShapes.at(0)->rotationangle()+10);
////    expand();update();

//}
void DrawAreaWidget::moveToTop(){
    if (pickedShapes.size()==0) return;
    TopAction* action=new TopAction();
    foreach(shared_ptr<GeneralShape> sp,pickedShapes){
        action->shapes.append(sp);
        action->indexOfShapes.append(shapes.indexOf(sp));
    del(sp);
    shapes.append(sp);
    }
    addaction(static_cast<AbstractAction*>(action));
    update();
}

void DrawAreaWidget::moveToBottom(){
    if (pickedShapes.size()==0) return;
    BottomAction* action=new BottomAction();
    for(int i=pickedShapes.size()-1;i>=0;i--) {
        action->shapes.append(pickedShapes.at(i));
        action->indexOfShapes.append(shapes.indexOf(pickedShapes.at(i)));
    del(pickedShapes.at(i));
    shapes.prepend(pickedShapes.at(i));
    }
    //action->shapes=pickedShapes;
    addaction(static_cast<AbstractAction*>(action));
    update();
}

void DrawAreaWidget::addfile(){
    openasadd=true;
    open();
}
void DrawAreaWidget::opennew(){
    openasadd=false;
    open();
}
//void DrawAreaWidget::openOldFileFormat(){
//    openasadd=false;
//    openold();
//}

void DrawAreaWidget::addshape(shared_ptr<GeneralShape> shape){
    shapes.append(shape);
    changed=true;
}

//void DrawAreaWidget::closeEvent(QCloseEvent *event){
//    qDebug()<<"close";
//    if(maybeSave())
//        event->accept();
//    else
//        event->ignore();
//}

bool DrawAreaWidget::maybeSave(){//成功保存，或明确放弃保存，或根本没什么可以保存的，返回true。
    if (!changed) return true;
    QMessageBox msgBox;
    msgBox.setText("Unsaved change will be lost!");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    //QMessageBox::Discard
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:{
        save();
        if( !saved) return false;
        else return true;
        // Save was clicked
        break;
    }
    case QMessageBox::Discard:{
        return true;
        // Don't Save was clicked
        break;
    }
    case QMessageBox::Cancel:{
        return false;
        // Cancel was clicked
        break;
    }
    default:
        // should never be reached
        break;
    }
    return true;
}
//void DrawAreaWidget::openold(){
//    if (!openasadd){
//        if (!maybeSave()) return;
//    }



//    QStringList files;
//    QFileDialog dlg(this,"Open file");
//    dlg.setAcceptMode(QFileDialog::AcceptOpen);
//    dlg.setNameFilter("INI (*.ini   *.inf)");
//    dlg.exec();
//    if (dlg.result()==QDialog::Accepted)
//        files = dlg.selectedFiles();
//    else
//        return;
//    if(files.isEmpty())
//        return;
//    if (!openasadd) init();
//    QFile openFile(files.at(0));
//    openFile.open(QFile::ReadOnly | QFile::Text);
//    QTextStream in(&openFile);
//    QString line;
//    while(!in.atEnd())
//    {
//        line=in.readLine();
//        add(line);
//    }
//}

void DrawAreaWidget::openfile(QString file){
    QFile openFile(file);

    if(!openFile.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&openFile))
    {
        openFile.close();
        return;
    }
    openFile.close();

    QDomElement root=doc.documentElement(); //返回根节点
    QDomElement e=root.toElement();
    if (e.nodeName()!="shapes"){
        QMessageBox msg;
        msg.setText("The document is unknown.");
          msg.exec();
          return;
    }
    if (e.hasAttribute("red")){
        int r,g,b;
        r=e.attribute("red").toInt();
        g=e.attribute("green").toInt();
        b=e.attribute("blue").toInt();
        backcolor=QColor(r,g,b);
    }
    ////
    shared_ptr<Combo> tmp=shared_ptr<Combo>(new Combo);//mem leak
    tmp->setShapes(root);
    //qDebug()<<"setshapes done";
    //shapes=tmp->shapes;

    if (openasadd){
        AbstractAction* action = new AddAction;
        action->shapes=static_pointer_cast<Combo>(tmp->copyPaste())->shapes;
        if(action->shapes.size()>0) addaction(action);
        openasadd=false;
    }
    else{
        filename=file;
        shapes =static_pointer_cast<Combo>(tmp->copyPaste())->shapes;
        emit categoryChanged();
    }
    // tmp;
    expand();update();

}


void DrawAreaWidget::open(){
    if (!openasadd){
        if (!maybeSave()) return;
    }



    QStringList files;
    QFileDialog dlg(this,"Open file");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setNameFilter("XML (*.xml   *.ini   *.inf)");
    dlg.exec();
    if (dlg.result()==QDialog::Accepted)
        files = dlg.selectedFiles();
    else
        return;
    if(files.isEmpty())
        return;
    if (!openasadd) init();
    openfile(files.at(0));


}



void DrawAreaWidget::saveAs(){
    saved=false;
    QStringList files;
     QFileDialog dlg(this,"Save file");
     dlg.setAcceptMode(QFileDialog::AcceptSave);
     dlg.setNameFilter("XML (*.xml   *.inf   *.ini )");
     dlg.exec();
     if (dlg.result()==QDialog::Accepted)
         files = dlg.selectedFiles();
     else
         return;
     if(files.isEmpty())    return;
     QFile saveFile(files.at(0));
     if(!saveFile.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
             return;
     QDomDocument doc;
     QDomProcessingInstruction instruction; //添加处理命令
     instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
     doc.appendChild(instruction);
     //添加根节点


     Combo* tmp=new Combo;
     tmp->shapes=shapes;
     QDomElement draw=tmp->toElement();
     //QDomElement draw=doc.createElement("draw");
     draw.setAttribute("red",backcolor.red());
     draw.setAttribute("green",backcolor.green());
     draw.setAttribute("blue",backcolor.blue());
     doc.appendChild(draw);

     QTextStream out_stream(&saveFile);
     doc.save(out_stream,4); //缩进4格
     saveFile.close();


     saved=true;
     changed=false;
     saveIndex=undoList.index();
     filename=files.at(0);
     emit categoryChanged();

}

void DrawAreaWidget::save(){
    saved=false;
    QString savefilename=filename;

    if (!QFile(savefilename).exists()){

   QStringList files;
    QFileDialog dlg(this,"Save file");
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setNameFilter("XML (*.xml   *.inf   *.ini )");
    dlg.exec();
    if (dlg.result()==QDialog::Accepted)
        files = dlg.selectedFiles();
    else
        return;
    if(files.isEmpty())    return;

     savefilename=files.at(0);
    }
    QFile saveFile(savefilename);
    if(!saveFile.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
            return;
    QDomDocument doc;
    QDomProcessingInstruction instruction; //添加处理命令
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //添加根节点


    Combo* tmp=new Combo;
    tmp->shapes=shapes;
    QDomElement draw=tmp->toElement();
    //QDomElement draw=doc.createElement("draw");
    draw.setAttribute("red",backcolor.red());
    draw.setAttribute("green",backcolor.green());
    draw.setAttribute("blue",backcolor.blue());
    doc.appendChild(draw);

    QTextStream out_stream(&saveFile);
    doc.save(out_stream,4); //缩进4格
    saveFile.close();


    saved=true;
    changed=false;
    saveIndex=undoList.index();
    filename=savefilename;
    emit categoryChanged();

}


void DrawAreaWidget::fitcanvas(){
    //zoom();
    if (shapes.size()==0) return;
    //qDebug()<<"shapes.size()="<<shapes.size();
    qreal MINX,MINY,MAXX,MAXY;
    MINX=shapes.at(0)->minx;
    MINY=shapes.at(0)->miny;
    MAXX=shapes.at(0)->maxx;
    MAXY=shapes.at(0)->maxy;
    foreach(shared_ptr<GeneralShape> sp,shapes){
        MINX=min(MINX,sp->minx);
        MINY=min(MINY,sp->miny);
        MAXX=max(MAXX,sp->maxx);
        MAXY=max(MAXY,sp->maxy);
    }
    MINX=MINX*zoomRatio;
    MINY=MINY*zoomRatio;
    MAXX=MAXX*zoomRatio;
    MAXY=MAXY*zoomRatio;
    dx=-MINX;
    dy=-MINY;
    setFixedSize(MAXX-MINX,MAXY-MINY);
    expand();
    update();
    //qDebug()<<MINX<<MINY<<MAXX<<MAXY;

}

void DrawAreaWidget::paintEvent(QPaintEvent *)
{
    QPalette palette;

    palette.setColor(QPalette::Base, QColor(backcolor));

    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));

    this->setPalette(palette);
    //expand();
    QPainter painter(this);
    //抗锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);//QPainter::TextAntialiasing,
    painter.translate(dx,dy);
    foreach(shared_ptr<GeneralShape> sp,shapes){
        sp->setPen(QPen(Qt::black));
    }

    foreach(shared_ptr<GeneralShape> sp,pickedShapes){
        sp->setPen(QPen(Qt::red,3));
        //if(sp->name=="Text") sp->setBrush();
    }
    foreach(shared_ptr<GeneralShape> sp,shapes){
        sp->draw(painter,zoomRatio);
        if(pickedShapes.indexOf(sp)>=0){
            sp->drawClosure(painter,zoomRatio);
        }
    }

//    foreach(shared_ptr<GeneralShape> sp,pickedShapes){
//        sp->drawClosure(painter,zoomRatio);
//    }

    if (pickRect!=NULL){
        QPen pen;  // creates a default pen

        pen.setStyle(Qt::DashDotLine);
        pickRect->setPen(pen);
        pickRect->draw(painter,zoomRatio);
    }

    painter.translate(-dx,-dy);
}
void DrawAreaWidget::contextMenuEvent( QContextMenuEvent * event ){
    if ( pickedShapes.size()==1 ){
        QMenu *menu = new QMenu();

        menu->addAction(actionMoveToTop);

        menu->addAction(actionMoveToBottom);

         menu->addAction(actionSetBrush);

        menu->exec(event->globalPos());
        // menu;
    }

}

void DrawAreaWidget::mouseDoubleClickEvent(QMouseEvent *event){
    //qDebug()<<"mouseDoubleClickEvent";
    if (currentCategory!=PickCategory){
        mousePressEvent(event);
        return;
    }
    if (currentCategory==PickCategory ){
        if ( pickedShapes.size()==1 ){
            if (pickedShapes.at(0)->name()=="Text"){

                shared_ptr<Text > pm = dynamic_pointer_cast< Text >( pickedShapes.at(0) );
                if ( pm ) {
                    GetTextDialog* gtd=new GetTextDialog;
                    gtd->setText(pm->text());
                    if (gtd->exec()==QDialog::Accepted){
                        pm->setText(gtd->text);
                    }

                    QFontDialog* fd=new QFontDialog;

                    fd->setCurrentFont(pm->myfont);

                    if (fd->exec()==QDialog::Accepted){
                        pm->setFont(fd->selectedFont()) ;
                    }
                    //pickedShapes.at(0)->setBrush(gtd->getColor());
                    expand();update();
                }


            }


            setBrush(pickedShapes.at(0));


        }
        else{//设置背景颜色
            QColorDialog* gtd=new QColorDialog;
            //qDebug()<<pickedShapes.at(0)->brush;
                gtd->setCurrentColor(backcolor);
            if (gtd->exec()==QDialog::Accepted){
                backcolor=gtd->selectedColor();
            }
            //pickedShapes.at(0)->setBrush(gtd->getColor());
            update();
        }
    }
}
void DrawAreaWidget::setBrush(){
    if ( pickedShapes.size()==1 ){
        setBrush(pickedShapes.at(0));
    }
}

void DrawAreaWidget::setBrush(shared_ptr<GeneralShape> sp){
    QColorDialog* gtd=new QColorDialog;
    //qDebug()<<pickedShapes.at(0)->brush;
    //gtd->setCurrentColor(sp->brush.color());
    if (sp->brush.style()!=Qt::NoBrush){
        gtd->setCurrentColor(sp->brush.color());
    }
    SetBrushAction* action=new SetBrushAction;
    action->oldbrush=sp->brush;
    action->shapes.append(sp);
    action->allShapes=&shapes;
    if (gtd->exec()==QDialog::Accepted){
        //sp->setBrush(gtd->selectedColor() );
        action->newbrush=gtd->selectedColor();
        addaction(action);
    }else{
        delete action;
    }
    //pickedShapes.at(0)->setBrush(gtd->getColor());
    update();
}

void DrawAreaWidget::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"mousePressEvent";
    //QPoint realPoint=event->pos()-QPoint(dx,dy);
    QPointF realPoint=(event->pos()-QPointF(dx,dy))/zoomRatio;
    switch(currentCategory)
    {
    case CurveCategory:
    case CloseCurveCategory:
    {
        if (currentCategory==CurveCategory) currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Curve>(new Curve));//mem leak
        else currentShape = static_pointer_cast<GeneralShape>(shared_ptr<CloseCurve>(new CloseCurve));
        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }

    case PalmCategory:{
        setMouseTracking(false);
        startCursorPoint=event->pos();
        endCursorPoint=event->pos();
        setCursor(Qt::ClosedHandCursor);
        break;
    }
    case PickCategory:
    {
        if (event->button()==Qt::RightButton){
            pickedShapes.clear();
            //pickedShape=NULL;
//            foreach(shared_ptr<GeneralShape> sp,shapes){
//                sp->setPen(QPen(Qt::black));
//            }
            update();
            //break;
        }
        setMouseTracking(false);

        if (pickedShapes.size()==1){//rotate or scale pickedShape
            if (isRotationHandlePoint(realPoint,pickedShapes.at(0))){//rotate pickedShape

                qDebug()<<"rotate pickedShape";
                currentMouseHanded=RotationPoint;
                endPoint=startPoint=realPoint;
                pickedShapestartRotationangle=pickedShapes.at(0)->Rotationangle;
                break;

            }
            if (isScaleHandlePoint(realPoint,pickedShapes.at(0))){//scale pickedShape
                qDebug()<<"scale pickedShape";
                currentMouseHanded=ScalePoint;
                //startPoint=realPoint;
                startPoint=pickedShapes.at(0)->scaleHandlePoint();
                pickedShapestartsx=pickedShapes.at(0)->getsx();
                pickedShapestartsy=pickedShapes.at(0)->getsy();
                break;
            }
            currentMouseHanded=None;

            //qDebug()<<"nothing pickedShape";
        }
        if (pickedShapes.size()>0){
            if (inRange(realPoint,pickedShapes)){//drag mode
                startPoint=realPoint;
                endPoint=realPoint;
                break;
            }
            else{
                pickedShapes.clear();
            }
        }
        currentMouseHanded=None;
//        foreach(shared_ptr<GeneralShape> sp,shapes){
//            sp->setPen(QPen(Qt::black));
//        }
        update();
        pickedShapes=pickShape(realPoint);
//        qDebug()<<pickedShapes.at(0)->name;//导致内存错误！！！
//        qDebug()<<pickedShapes.at(0)->points;
        if (pickedShapes.size()==1) {
            //qDebug()<<"pickedShapes.size()==1";


            endPoint=startPoint=realPoint;
            //pickedShapes.at(0)->setPen(QPen(Qt::red,3));
            update();
        }
        else{//框选模式
            //qDebug()<<"pickedShape==NULL";
            startPoint=realPoint;
            // pickRect;
            pickRect=shared_ptr<Rect>(new Rect);//mem leak
            pickRect->addPoint(realPoint);
            pickRect->addPoint(realPoint);

        }
        //else //qDebug()<<"Nothing";
        break;
    }
    case CircleCategory:{
        setMouseTracking(false);
        currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Circle>(new Circle));//new Circle;

        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }

    case RectCategory:{
        setMouseTracking(false);
        currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Rect>(new Rect));//new Rect;

        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }
    case EllipseCategory:{
        setMouseTracking(false);
        currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Ellipse>(new Ellipse));//new Ellipse;

        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }
    case TextCategory:{
        setMouseTracking(false);
        currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Text>(new Text));//new Text;

        isLeftButtonPressed = true;



        break;
    }
    }


}
void DrawAreaWidget::addaction(AbstractAction* act){
    act->allShapes=&shapes;
    //act->pickedShapes=&pickedShapes;
    undoList.push(act);//
    pickedShapes=act->pickedShapes;



//    while (actionList.size()> actionindex){
//        delete actionList.last();
//        actionList.pop_back();
//    }

//    actionList.append( act);
//    actionindex++;
    //qDebug()<<"add action"<<act->actiontype();
    changed=true;
    update();
    emit categoryChanged();
}

void DrawAreaWidget::mouseReleaseEvent(QMouseEvent *event)
{

    //qDebug()<<"mouseReleaseEvent";

    //QPoint realPoint=event->pos()-QPoint(dx,dy);
    QPointF realPoint=(event->pos()-QPointF(dx,dy))/zoomRatio;
    switch(currentCategory)
    {
    case CloseCurveCategory:
    case CurveCategory:{
        //currentShape->addPoint(realPoint);
        isLeftButtonPressed = false;
//        if (currentShape->isEmpty()){ shapes.removeLast();}
//        else{
//            AbstractAction *action=new AddAction(currentShape);
//            addaction(action);

//        }
//        currentShape = NULL;
        finishcurrentShape();
        break;
    }
    case PolylineCategory:
    {
        setMouseTracking(true);
        if (currentShape==NULL){
            currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Polyline>(new Polyline));//new Polyline;
            shapes.append(currentShape);
            currentShape->addPoint(realPoint);
            //currentShape->addPoint(realPoint);
            //currentShape->addPoint(event->pos());
        }
        isLeftButtonPressed = true;


        //currentShape->setCurPoint(event->pos());
        currentShape->removeLastPoint();
        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        update();
        if (event->button()==Qt::RightButton){//end one Polyline
            currentShape->removeLastPoint();
//            if (currentShape->isEmpty()){ shapes.removeLast();}
//            else{
//                AbstractAction *action=new AddAction(currentShape);
//                addaction(action);
//            }
//            currentShape = NULL;
            finishcurrentShape();
            //currentCategory = PolylineCategory;
            isLeftButtonPressed = false;
        }
        break;
    }
    case PolygonCategory:
    {
        setMouseTracking(true);
        if (currentShape==NULL){
            currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Polygon>(new Polygon));//new Polygon;
            shapes.append(currentShape);
            currentShape->addPoint(realPoint);
        }
        isLeftButtonPressed = true;

        //currentShape->setBeginPoint(event->pos());
        //shapes.append(currentShape);
        //break;
        currentShape->removeLastPoint();
        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);

        update();
        if (event->button()==Qt::RightButton){//end one Polyline
            currentShape->removeLastPoint();
//            if (currentShape->isEmpty()){ shapes.removeLast();}
//            else{
//                AbstractAction *action=new AddAction(currentShape);
//                addaction(action);
//            }
//            currentShape = NULL;
            finishcurrentShape();
            //currentCategory = PolylineCategory;
            isLeftButtonPressed = false;
        }
        break;
    }
    case PalmCategory:{
        //startCursorPoint=endCursorPoint;
        endCursorPoint=event->pos();
        int vvalue=vBar->value();
        int hvalue=hBar->value();
        vBar->setValue(vvalue-(endCursorPoint-startCursorPoint).y());
        hBar->setValue(hvalue-(endCursorPoint-startCursorPoint).x());
        setCursor(Qt::OpenHandCursor);
        update();
        break;
    }
    case PickCategory:{

        if (pickedShapes.size()==0) {//框选
            // pickRect;
            pickRect=NULL;
            //qDebug()<<"";
            update();
            endPoint=realPoint;
            pickedShapes=pickShape(startPoint,endPoint);
            if (pickedShapes.size()>0) {

                update();
            }
            break;
        }
        if (currentMouseHanded==RotationPoint){//Rotation
            currentMouseHanded=None;
            qDebug()<<"RotationPoint";
            QPointF centralpoint=QPointF((pickedShapes.at(0)->minx+pickedShapes.at(0)->maxx)/2,
                                         (pickedShapes.at(0)->miny+pickedShapes.at(0)->maxy)/2);

            qreal sita=acos( -(realPoint.y()-centralpoint.y()) / sqrt(
                       (realPoint.x()-centralpoint.x())*
                       (realPoint.x()-centralpoint.x())+
                       (realPoint.y()-centralpoint.y())*
                       (realPoint.y()-centralpoint.y()) ) );
            if (realPoint.x()-centralpoint.x()<0){
                sita=-sita;
            }
            if (pickedShapes.at(0)->getsy()<0){
                sita=sita+M_PI;//解决图形倒置时的bug
            }
            pickedShapes.at(0)->setRotationangle(sita/M_PI*180);

            EditAction* action = new EditAction();
            action->shapes.append(pickedShapes.at(0));
            action->oldRotationangle=pickedShapestartRotationangle;
            action->newRotationangle=pickedShapes.at(0)->Rotationangle;
            AbstractAction* tmp=static_cast<AbstractAction*>(action);
            //tmp->pickedShapes=&pickedShapes;
            tmp->undo();
            addaction(tmp);
            //expand();
            update();
            break;
        }
        if (currentMouseHanded==ScalePoint){//scale,NEED REWRITE!!!
            currentMouseHanded=None;
            qDebug()<<"ScalePoint";
            endPoint=realPoint;
//            pickedShapes.at(0)->sx=pickedShapes.at(0)->sx*(1+(endPoint-startPoint).x()*zoomRatio/100);
//            pickedShapes.at(0)->sy=pickedShapes.at(0)->sy*(1+(endPoint-startPoint).y()*zoomRatio/100);
            QPointF centralpoint=QPointF((pickedShapes.at(0)->minx+pickedShapes.at(0)->maxx)/2,
                                         (pickedShapes.at(0)->miny+pickedShapes.at(0)->maxy)/2);
            QPointF p=startPoint-centralpoint;
            QPointF p1=endPoint-centralpoint;
            p=rotated(p,pickedShapes.at(0)->Rotationangle/180*M_PI);//计算机绘图坐标系，点逆时针旋转（或坐标系顺时针旋转），弧度制！！！
            p1=rotated(p1,pickedShapes.at(0)->Rotationangle/180*M_PI);
            pickedShapes.at(0)->setsx(pickedShapestartsx*p1.x()/p.x());
            pickedShapes.at(0)->setsy(pickedShapestartsy*p1.y()/p.y());


            EditAction* action = new EditAction();
            action->shapes.append(pickedShapes.at(0));
            action->dsx=pickedShapes.at(0)->getsx()/pickedShapestartsx;
            action->dsy=pickedShapes.at(0)->getsy()/pickedShapestartsy;
            //action->oldRotationangle=pickedShapestartRotationangle;
            //action->newRotationangle=pickedShapes.at(0)->Rotationangle;
            AbstractAction* tmp=static_cast<AbstractAction*>(action);
            //tmp->pickedShapes=&pickedShapes;
            tmp->undo();
            addaction(tmp);

            //expand();
            update();
            break;
        }
        if (pickedShapes.size()>0){

            foreach(shared_ptr<GeneralShape> sp,pickedShapes){
                sp->drag(realPoint-endPoint);

            }
            EditAction* action = new EditAction();
            action->shapes=pickedShapes;
            action->dpoint=realPoint-startPoint;
            AbstractAction* tmp=static_cast<AbstractAction*>(action);
            if (action->dpoint!=QPointF(0,0)) {
                //tmp->pickedShapes=&pickedShapes;
                tmp->undo();
                addaction(tmp);
            }
            else{
                delete action;
            }
            endPoint=realPoint;
            //expand();
            update();
            break;
        }

        break;
    }
    case CircleCategory:
    {
        currentShape->removeLastPoint();
        currentShape->addPoint(realPoint);
        isLeftButtonPressed = false;
//        if (currentShape->isEmpty()){ shapes.removeLast();}
//        else{
//            AbstractAction *action=new AddAction(currentShape);
//            addaction(action);
//        }
//        currentShape = NULL;
        finishcurrentShape();
        update();
        break;
    }
    case TextCategory:{//input qstring
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        GetTextDialog* gtd=new GetTextDialog;
//        QString currentText="hello world!";
        gtd->setText("hello world!");
        //gtd->ui

        if (gtd->exec()==QDialog::Accepted){
            shared_ptr<Text> pm = dynamic_pointer_cast< Text >( currentShape );
            if ( pm ) {
                //pm->setText(currentText);
                pm->setText(gtd->text);
            }
            //currentShape->setText(currentText );
        }
//        if (currentShape->isEmpty()){ shapes.removeLast();}
//        else{
//            AbstractAction *action=new AddAction(currentShape);
//            addaction(action);
//        }
//        currentShape = NULL;
        finishcurrentShape();
        expand();update();
        break;
    }
    case EllipseCategory:
    case RectCategory://
        /*one double click will triger 4 events:
         * mousePressEvent
        *mouseReleaseEvent
        *mouseDoubleClickEvent
        *mouseReleaseEvent
         * the last mouseReleaseEvent with no  mousePressEvent will cause error
         */
    {
        currentShape->removeLastPoint();
        currentShape->addPoint(realPoint);
        isLeftButtonPressed = false;
//        if (currentShape->isEmpty()){ shapes.removeLast();}
//        else{
//            AbstractAction *action=new AddAction(currentShape);
//            addaction(action);
//        }
//        currentShape = NULL;
        finishcurrentShape();
        update();
        break;
    }
    }
    emit categoryChanged();
}
void DrawAreaWidget::keyPressEvent(QKeyEvent *event){
    //qDebug()<<"press";
    if (pickedShapes.size()==0) return;
    if (event->key()==Qt::Key_Delete){

        if (pickedShapes.size()>0){
            DeleteAction *action=new DeleteAction();
            qDebug()<<action->shapes.size();
            foreach(shared_ptr<GeneralShape> sp,pickedShapes){

                action->shapes.append(sp);
                action->indexOfShapes.append(shapes.indexOf(sp));
                del(sp);
            }
            addaction(static_cast<AbstractAction*>(action));
            pickedShapes.clear();
            update();
        }
        return;
    }

    switch (event->key())
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:{

        if (!event->isAutoRepeat()) {
            startPoint=pickedShapes.at(0)->points.at(0);
        }
        if (event->key()==Qt::Key_W){
            //qDebug()<<"Up pressed";
            pickedMove(0,-1/zoomRatio);
        }else
            if (event->key()==Qt::Key_S){
                //qDebug()<<"Up pressed";
                pickedMove(0,1/zoomRatio);
            }else
                if (event->key()==Qt::Key_A){
                    //qDebug()<<"Up pressed";
                    pickedMove(-1/zoomRatio,0);
                }else
                    if (event->key()==Qt::Key_D){
                        //qDebug()<<"Up pressed";
                        pickedMove(1/zoomRatio,0);
                    }else return;
    }

}
void DrawAreaWidget::keyReleaseEvent(QKeyEvent *event){
    if (pickedShapes.size()==0) return;
    if(event->isAutoRepeat())
    {
        event->ignore();
        return;
    }
    switch (event->key())
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_A:
    case Qt::Key_D:{
        endPoint=pickedShapes.at(0)->points.at(0);
        EditAction* action = new EditAction();
        action->shapes=pickedShapes;
        action->dpoint=endPoint-startPoint;
        AbstractAction* tmp=static_cast<AbstractAction*>(action);
        if (action->dpoint!=QPointF(0,0)) {
            //tmp->pickedShapes=&pickedShapes;
            tmp->undo();
            addaction(tmp);
        }
        else{
            delete action;
        }
    }
}

void DrawAreaWidget::pickedMove(qreal x, qreal y){

    if (pickedShapes.size()>0){
        foreach(shared_ptr<GeneralShape> sp,pickedShapes){
            sp->drag(QPointF(x,y));
        }

        //expand();
        update();
    }
}

void DrawAreaWidget::del(shared_ptr<GeneralShape> sp){
    shapes.removeOne(sp);
    //update();
}

void DrawAreaWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF realPoint=(event->pos()-QPointF(dx,dy))/zoomRatio;
    //if (event->pos().x()>this->width() || event->pos().y()>this->height()) expand(event->pos());
    switch(currentCategory)
    {
    case CloseCurveCategory:
    case CurveCategory:{
        if(currentShape && isLeftButtonPressed)
        {
            currentShape->addPoint(realPoint);
            //shapes.append(currentShape);
            //expand();
            update();
        }
        break;
    }
    case PolylineCategory:{
        if (currentShape!=NULL ){
            //currentShape->setCurPoint(event->pos());
            currentShape->removeLastPoint();
            currentShape->addPoint(realPoint);
            //expand();
            update();
        }
        break;
    }
    case PolygonCategory:{
        if (currentShape!=NULL ){
            //currentShape->setCurPoint(event->pos());
            currentShape->removeLastPoint();
            currentShape->addPoint(realPoint);
            //expand();
            update();
        }
        break;
    }
    case PalmCategory:{
        //startCursorPoint=endCursorPoint;
        //鼠标移动了，画布暂时还没跟上。
        endCursorPoint=event->pos();


        vBar->setValue(vBar->value()-(endCursorPoint-startCursorPoint).y());
        hBar->setValue(hBar->value()-(endCursorPoint-startCursorPoint).x());
        //滚动条拖动后鼠标在窗口的位置没变，但在画布上的位置变了
        //startCursorPoint=pos();
        update();
        break;
    }
    case PickCategory:{

        if (pickedShapes.size()==0) {//框选模式
            pickRect->removeLastPoint();
            pickRect->addPoint(realPoint);
            //expand();
            update();
            return;
        }
        if (currentMouseHanded==RotationPoint){//Rotation
            //qDebug()<<"RotationPoint";
            QPointF centralpoint=QPointF((pickedShapes.at(0)->minx+pickedShapes.at(0)->maxx)/2,
                                         (pickedShapes.at(0)->miny+pickedShapes.at(0)->maxy)/2);

            qreal sita=acos( -(realPoint.y()-centralpoint.y()) / sqrt(
                       (realPoint.x()-centralpoint.x())*
                       (realPoint.x()-centralpoint.x())+
                       (realPoint.y()-centralpoint.y())*
                       (realPoint.y()-centralpoint.y()) ) );
            if (realPoint.x()-centralpoint.x()<0){
                sita=-sita;
            }
            if (pickedShapes.at(0)->getsy()<0){
                sita=sita+M_PI;//解决图形倒置时的bug
            }
            pickedShapes.at(0)->setRotationangle(sita/M_PI*180);
            //expand();
            update();
            break;
        }
        if (currentMouseHanded==ScalePoint){//scale,NEED REWRITE!!!
            //qDebug()<<"ScalePoint";
            endPoint=realPoint;
//            pickedShapes.at(0)->sx=pickedShapes.at(0)->sx*(1+(endPoint-startPoint).x()*zoomRatio/100);
//            pickedShapes.at(0)->sy=pickedShapes.at(0)->sy*(1+(endPoint-startPoint).y()*zoomRatio/100);
            QPointF centralpoint=QPointF((pickedShapes.at(0)->minx+pickedShapes.at(0)->maxx)/2,
                                         (pickedShapes.at(0)->miny+pickedShapes.at(0)->maxy)/2);
            QPointF p=startPoint-centralpoint;
            QPointF p1=endPoint-centralpoint;
            p=rotated(p,pickedShapes.at(0)->Rotationangle/180*M_PI);//计算机绘图坐标系，点逆时针旋转（或坐标系顺时针旋转），弧度制！！！
            p1=rotated(p1,pickedShapes.at(0)->Rotationangle/180*M_PI);
            pickedShapes.at(0)->setsx(pickedShapestartsx*p1.x()/p.x());
            pickedShapes.at(0)->setsy(pickedShapestartsy*p1.y()/p.y());

            //expand();
            update();
            return;
        }
        if (pickedShapes.size()>0){
            //startPoint=endPoint;

            //pickedShapes->drag(endPoint-startPoint);
            foreach(shared_ptr<GeneralShape> sp,pickedShapes){
                sp->drag(realPoint-endPoint);
            }
            endPoint=realPoint;
            //expand();
            update();
            break;
        }

    }
    case CircleCategory:{
        if (currentShape!=NULL ){
            //currentShape->setCurPoint(event->pos());
            currentShape->removeLastPoint();
            currentShape->addPoint(realPoint);
            //expand();
            update();
        }
        break;
    }
    case EllipseCategory:

    case RectCategory:{
        if (currentShape!=NULL ){
            //currentShape->setCurPoint(event->pos());
            currentShape->removeLastPoint();
            currentShape->addPoint(realPoint);
            //expand();
            update();
        }
        break;
    }
    case TextCategory:{
        break;
    }
    }
}

void DrawAreaWidget::setCategory(Category c){
    setMouseTracking(false);
    finishcurrentShape();
    if (c==PalmCategory){
        setCursor(Qt::OpenHandCursor);
    }
    else{
        setCursor(Qt::ArrowCursor);
    }
    if (c!=PickCategory){
        pickedShapes.clear();
        update();
    }
    currentCategory=c;
    emit categoryChanged();
}
void DrawAreaWidget::finishcurrentShape(){
    if (currentShape!=NULL){
        shapes.removeLast();
        if (currentShape->isEmpty()){


            // currentShape;
        }
        else{
            AbstractAction *action=new AddAction;//(currentShape);
            action->shapes.append(currentShape);
            addaction(action);
        }
    }

    currentShape = NULL;
    update();
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::pickShape(QPointF point){

    QList<shared_ptr<GeneralShape>> tmp;
    for(int i=shapes.size()-1;i>=0;i--){//这种方式选取顺序为从上面往底下。
        if (shapes.at(i)->minDistance(point)*zoomRatio<4) {
            tmp.append(shapes.at(i)) ;
            break;
        }
    }
    return tmp;
}
QList<shared_ptr<GeneralShape>> DrawAreaWidget::pickShape(QPointF p0,QPointF p1){
    QList<shared_ptr<GeneralShape>> tmp;
    foreach(shared_ptr<GeneralShape> sp,shapes){
        if (sp->inRange(p0,p1)) tmp.append(sp);
    }
    return tmp;
}
void DrawAreaWidget::zoomone(){
    zoomRatio=1;
    dozoom();
}

void DrawAreaWidget::dozoom(){

    double pdx=dx;
    double pdy=dy;

    dx= pdx*zoomRatio/previouszoomRatio;
    dy= pdy*zoomRatio/previouszoomRatio;
    double hv=hBar->value();
    double vv=vBar->value();
    setFixedSize(  width()*zoomRatio/previouszoomRatio ,
                   height()*zoomRatio/previouszoomRatio);
    hBar->setValue((hv+windowwidth/2)*zoomRatio/previouszoomRatio-windowwidth/2);
    vBar->setValue((vv+windowheight/2)*zoomRatio/previouszoomRatio-windowheight/2);
    expand();
    update();
    previouszoomRatio=zoomRatio;
}
void DrawAreaWidget::zoom(double ratio){
    zoomRatio=zoomRatio*ratio;
    dozoom();

}
bool DrawAreaWidget::isScaleHandlePoint(QPointF realPoint,shared_ptr<GeneralShape> pickedShape){
    QPointF handlePoint=pickedShape->scaleHandlePoint();
    //qDebug()<<handlePoint;
    double x=handlePoint.x();
    double y=handlePoint.y();
    //qDebug()<<x-realPoint.x()<<y-realPoint.y();
    if (abs(x-realPoint.x())*zoomRatio<5 && abs(y-realPoint.y())*zoomRatio<5) return true;
    else return false;
}

bool DrawAreaWidget::isRotationHandlePoint(QPointF realPoint,shared_ptr<GeneralShape> pickedShape){
    QPointF handlePoint=pickedShape->rotationHandlePoint();
    //qDebug()<<handlePoint;
    double sy=pickedShapes.at(0)->getsy();
    double x=handlePoint.x()+sy/abs(sy)*lenthOfRotationHandleLine/zoomRatio*sin(pickedShapes.at(0)->rotationangle()/180*M_PI);
    double y=handlePoint.y()-sy/abs(sy)*lenthOfRotationHandleLine/zoomRatio*cos(pickedShapes.at(0)->rotationangle()/180*M_PI);
    //qDebug()<<x-realPoint.x()<<y-realPoint.y();
    if (abs(x-realPoint.x())*zoomRatio<5 && abs(y-realPoint.y())*zoomRatio<5) return true;
    else return false;
}
void DrawAreaWidget::undo(){
    undoList.undo();
    QUndoCommand* tmp=const_cast<QUndoCommand*>(undoList.command(undoList.index()));
    pickedShapes=((dynamic_cast<AbstractAction*>(tmp))->pickedShapes);
    if (pickedShapes.isEmpty() && undoList.index()>0){
        QUndoCommand* tmp=const_cast<QUndoCommand*>(undoList.command(undoList.index()-1));
        pickedShapes=(dynamic_cast<AbstractAction*>(tmp))->pickedShapes;
    }




/*    if (actionindex==0) return;
//    AbstractAction* action=actionList.at(actionindex-1);
//    switch (action->actiontype()){
//    case Add:{
////        foreach (shared_ptr<GeneralShape>sp,action->shapes){
////        del(sp);
////        pickedShapes.removeOne(sp);
////        }
//        action->undo();
//        pickedShapes.clear();
//        actionindex--;
//        update();
//        break;
//    }
//    case Delete:{
//        DeleteAction* tmp=static_cast<DeleteAction*>(action);
//        for (int i=tmp->shapes.size()-1;i>=0;i--){
//        shapes.insert(tmp->indexOfShapes.at(i),tmp->shapes.at(i));
//        }
//        actionindex--;
//        update();
//        break;
//    }
//    case Edit:{
////        EditAction* tmp=static_cast<EditAction*>(action);
////        foreach (shared_ptr<GeneralShape>sp,tmp->shapes){
////        sp->drag(-tmp->dpoint);
////        if (tmp->newRotationangle!=tmp->oldRotationangle){
////            sp->setRotationangle(tmp->oldRotationangle);
////        }
////        sp->setsx(tmp->shapes.at(0)->getsx()/tmp->dsx);
////        sp->setsy(tmp->shapes.at(0)->getsy()/tmp->dsy);
////        }
//        action->undo();
//        actionindex--;
//        update();
//        break;
//    }
//    case Combine:{
////        CombineAction* tmp=static_cast<CombineAction*>(action);
////        for(int i=tmp->shapes.size()-1;i>=0;i--){
////            shapes.insert(tmp->indexOfShapes.at(i),tmp->shapes.at(i));
////        }
////        //qDebug()<<shapes.indexOf(tmp->com)<<shapes.size();
////        shapes.removeOne(tmp->com);
//        action->undo();
//        actionindex--;
//        pickedShapes=action->shapes;
//        update();
//        break;
//    }
//    case Divide:{
//        DivideAction* tmp=static_cast<DivideAction*>(action);
////        for(int i=0;i<tmp->shapes.size();i++){
////            shapes.removeOne(tmp->shapes.at(i));
////            //getIntoCombo(tmp->shapes.at(i),tmp->com);

////        }
////        //shapes.append(tmp->com);
////        shapes.insert(tmp->indexOfCom,tmp->com);
//        action->undo();
//        pickedShapes.clear();
//        pickedShapes.append(tmp->com);
//        actionindex--;
//        update();
//        break;
//    }
//    case Top:{
//        TopAction* tmp=static_cast<TopAction*>(action);
//        for(int i=tmp->shapes.size()-1;i>=0;i--){
//            shapes.removeOne(tmp->shapes.at(i));
//            shapes.insert(tmp->indexOfShapes.at(i),tmp->shapes.at(i));
//        }
//        actionindex--;
//        pickedShapes=tmp->shapes;
//        update();
//        break;
//    }
//    case Bottom:{
//        BottomAction* tmp=static_cast<BottomAction*>(action);
//        for(int i=tmp->shapes.size()-1;i>=0;i--){
//            shapes.removeOne(tmp->shapes.at(i));
//            shapes.insert(tmp->indexOfShapes.at(i),tmp->shapes.at(i));
//        }
//        actionindex--;
//        pickedShapes=tmp->shapes;
//        update();
//        break;
//    }
//    case SetBrush:{
//        //SetBrushAction* tmp=static_cast<SetBrushAction*>(action);
//        action->undo();
////        foreach(shared_ptr<GeneralShape>sp,tmp->shapes){
////            sp->setBrush(tmp->oldbrush);
////        }
//        actionindex--;
//        pickedShapes=action->shapes;
//        update();
//        break;
//    }
//    }*/



    if (undoList.index()==saveIndex) {changed=false;}
    else {changed=true;}
    emit categoryChanged();
    update();
}
void DrawAreaWidget::redo(){
    undoList.redo();
    QUndoCommand* tmp=const_cast<QUndoCommand*>(undoList.command(undoList.index()-1));
    pickedShapes=((dynamic_cast<AbstractAction*>(tmp))->pickedShapes);


/*    if (actionindex==actionList.size()) return;
//    AbstractAction* action=actionList.at(actionindex);
//    switch (action->actiontype()){
//    case Add:{
////        foreach (shared_ptr<GeneralShape>sp,action->shapes){
////        addshape(sp);
////        }
//        action->redo();
//        actionindex++;
//        update();
//        break;
//    }
//    case Delete:{
//        foreach(shared_ptr<GeneralShape>sp, action->shapes){
//        del(sp);
//        }
//        actionindex++;
//        update();
//        break;
//    }
//    case Edit:{
//        //EditAction* tmp=static_cast<EditAction*>(action);
////        foreach (shared_ptr<GeneralShape>sp,tmp->shapes){
////        sp->drag(tmp->dpoint);
////        if (tmp->newRotationangle!=tmp->oldRotationangle){
////            sp->setRotationangle(tmp->newRotationangle);
////        }
////        sp->setsx(tmp->shapes.at(0)->getsx()*tmp->dsx);
////        sp->setsy(tmp->shapes.at(0)->getsy()*tmp->dsy);
////        }
//        action->redo();
//        actionindex++;
//        update();
//        break;
//    }
//    case Combine:{
//        CombineAction* tmp=static_cast<CombineAction*>(action);
//        action->redo();
//        pickedShapes.clear();
//        pickedShapes.append(tmp->com);
//        actionindex++;
//        update();
//        break;
//    }
//    case Divide:{
//        DivideAction* tmp=static_cast<DivideAction*>(action);
////        for(int i=0;i<tmp->shapes.size();i++){
////            int index=shapes.indexOf(static_cast<shared_ptr<GeneralShape>> (tmp->com));
////             shapes.insert(index,tmp->shapes.at(i));

////            //getOutOfCombo(tmp->shapes.at(i),tmp->com);

////        }
////        del(tmp->com);
//        action->redo();
//        pickedShapes=tmp->shapes;
//        actionindex++;
//        update();
//        break;
//    }
//    case Top:{
//        TopAction* tmp=static_cast<TopAction*>(action);
//        for(int i=0;i<tmp->shapes.size();i++){
//            shapes.removeOne(tmp->shapes.at(i));
//            shapes.append(tmp->shapes.at(i));
//        }
//        actionindex++;
//        pickedShapes=tmp->shapes;
//        update();
//        break;
//    }
//    case Bottom:{
//        BottomAction* tmp=static_cast<BottomAction*>(action);
//        for(int i=0;i<tmp->shapes.size();i++){
//            shapes.removeOne(tmp->shapes.at(i));
//            shapes.prepend(tmp->shapes.at(i));
//        }
//        actionindex++;
//        pickedShapes=tmp->shapes;
//        update();
//        break;
//    }
//    case SetBrush:{
//        action->redo();

//        actionindex++;
//        pickedShapes=action->shapes;
//        update();
//        break;
//    }
//    }*/
    if (undoList.index()==saveIndex) {changed=false;}
    else {changed=true;}
    emit categoryChanged();
    update();
}
void DrawAreaWidget::expand(){
    foreach(shared_ptr<GeneralShape> sp,shapes){
        sp->updateBand();
        int ddx,ddy;//
        ddx=ddy=0;
        if (sp->left*zoomRatio < -dx){  ddx=-sp->left*zoomRatio-dx; dx=dx+ddx;}
        if (sp->top*zoomRatio < -dy) {ddy=-sp->top*zoomRatio-dy; dy=dy+ddy;}
        //setGeometry ( geometry().x()-ddx,geometry().y()-ddy,width()+ddx,height()+ddy);
        //isScrolling=true;
        //emit scrollContentsBy(ddx, ddy);
        setFixedSize(width()+ddx,height()+ddy);
        vBar->setValue(vBar->value() + ddy);
        hBar->setValue(hBar->value() + ddx);
        //qDebug()<<"expand setValue!";
//        while(isScrolling){

//        }

        if (sp->right*zoomRatio+dx > width()) setFixedWidth(sp->right*zoomRatio+dx);
        if (sp->bottom*zoomRatio+dy > height()) setFixedHeight(sp->bottom*zoomRatio+dy);
    }
}

bool DrawAreaWidget::inRange(QPointF Point, QList<shared_ptr<GeneralShape>> sps){
    foreach(shared_ptr<GeneralShape> sp,sps){
        if (sp->minDistance(Point)*zoomRatio<4) return true;

    }

    return false;

}
void DrawAreaWidget::test(){
//    pickedShapes.at(0)->zoom(2);
//    update();
}
