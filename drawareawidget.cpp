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
    init();
}

void DrawAreaWidget::init()
{
    currentShape = NULL;
    pickedShapes.clear();
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
}

void DrawAreaWidget::combination(){
    if (pickedShapes.size()<=1) return;
    Combo* tmp=new Combo;
    foreach(GeneralShape * sp,pickedShapes){
        tmp->shapes.append(sp);
        del(sp);
    }
    tmp->updateRange();
    pickedShapes.clear();
    shapes.append(tmp);
    update();

}
void DrawAreaWidget::divide(){
    if (pickedShapes.size()!=1 || pickedShapes.at(0)->name!="Combo") return;
    Combo* tmp=dynamic_cast<Combo* >(pickedShapes.at(0));
    foreach(GeneralShape * sp,tmp->shapes){

        getOutOfCombo(sp,tmp);
    }
    del(pickedShapes.at(0));
    pickedShapes.removeAt(0);
    update();
}
void DrawAreaWidget::divideToEnd(){
    divideToEnd(pickedShapes.at(0));
    pickedShapes.removeAt(0);
    update();

}
void DrawAreaWidget::divideToEnd(GeneralShape* shape){
    if (shape->name!="Combo") return;
    Combo* tmp=dynamic_cast<Combo* >(shape);
    foreach(GeneralShape * sp,tmp->shapes){

        getOutOfCombo(sp,tmp);
        divideToEnd(sp);

    }
    del(shape);

}
void DrawAreaWidget::getOutOfCombo(GeneralShape* sp,Combo* tmp){
    QPointF spcentralPoint=sp->centralPoint();
    sp->drag(-spcentralPoint);
    sp->Rotationangle=sp->Rotationangle+tmp->Rotationangle;
    //sp->drag(tmp->centralPoint());
    sp->drag(rotated(spcentralPoint-tmp->centralPoint(),-tmp->Rotationangle/180*M_PI));//rotate done
    //sp->drag(-tmp->centralPoint());
    sp->zoom(tmp->sx);
    sp->drag(tmp->centralPoint());//scale done
    shapes.append(sp);
}

void DrawAreaWidget::copyPaste(){

    if (pickedShapes.size()>0) {
        QList<GeneralShape *> copiedshapes;
        foreach(GeneralShape* sp,pickedShapes){
            GeneralShape *tmp=sp->copyPaste();
            tmp->drag(QPointF(10,10));
            shapes.append(tmp);
            copiedshapes.append(tmp);
        }

        pickedShapes=copiedshapes;
        expand();update();
    }

}
QList<GeneralShape *> DrawAreaWidget::copy(){
    QList<GeneralShape *> copiedshapes;
    if (pickedShapes.size()>0) {

        foreach(GeneralShape* sp,pickedShapes){
            GeneralShape *tmp=sp->copyPaste();

            copiedshapes.append(tmp);
        }


    }
    return copiedshapes;
}
QList<GeneralShape *> DrawAreaWidget::cut(){
    QList<GeneralShape *> copiedshapes;
    if (pickedShapes.size()>0) {

        foreach(GeneralShape* sp,pickedShapes){
            GeneralShape *tmp=sp->copyPaste();

            copiedshapes.append(tmp);
        }
        foreach(GeneralShape* sp,pickedShapes){
            del(sp);
        }
        pickedShapes.clear();
        update();


    }
    return copiedshapes;
}
void DrawAreaWidget::paste(QList<GeneralShape *>*copyShapes){
    if (copyShapes->size()>0) {
        QList<GeneralShape *> copiedshapes;
        for(int i=1;i<=copyShapes->size();i++){
            GeneralShape *tmp=copyShapes->at(i-1)->copyPaste();
            tmp->drag(QPointF(10,10));
            shapes.append(tmp);
            copiedshapes.append(tmp);
            copyShapes->removeAt(i-1);
            copyShapes->insert(i-1,tmp);
            //sp=tmp;
        }
//        foreach(GeneralShape* sp,*copyShapes){
//            GeneralShape *tmp=sp->copyPaste();
//            tmp->drag(QPointF(10,10));
//            shapes.append(tmp);
//            copiedshapes.append(tmp);
//            //sp=tmp;
//        }
        //copyShapes=&copiedshapes;
        pickedShapes=copiedshapes;
        expand();update();
    }
}

void DrawAreaWidget::rotate(){
//    if (pickedShapes.size()!=1) return;
//    pickedShapes.at(0)->setRotationangle( pickedShapes.at(0)->rotationangle()+10);
//    expand();update();

}
void DrawAreaWidget::moveToTop(){
    if (pickedShapes.size()!=1) return;
    del(pickedShapes.at(0));
    shapes.append(pickedShapes.at(0));
    update();
}

void DrawAreaWidget::moveToBottom(){
    if (pickedShapes.size()!=1) return;
    del(pickedShapes.at(0));
    shapes.prepend(pickedShapes.at(0));
    update();
}
void DrawAreaWidget::changeToClose(){
    if (pickedShapes.size()!=1) return;
    GeneralShape* sp=NULL;
    if (pickedShapes.at(0)->name=="Curve"){
        //qDebug()<<"changeToClose";
        sp=new CloseCurve;


    }
    if (pickedShapes.at(0)->name=="Polyline"){
        sp=new Polygon;
    }
    if (sp==NULL) return;
    sp->points=pickedShapes.at(0)->points;
    sp->sx=pickedShapes.at(0)->sx;
    sp->sy=pickedShapes.at(0)->sy;
    sp->Rotationangle=pickedShapes.at(0)->Rotationangle;
    sp->updateRange();
    //pickedShape=sp;//need recode!!!
    del(pickedShapes.at(0));
    shapes.append(sp);
    //pickedShapes.at(0)=sp;
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
void DrawAreaWidget::openOldFileFormat(){
    openasadd=false;
    openold();
}

void DrawAreaWidget::addshape(GeneralShape * shape){
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
    if (shapes.size()==0) return true;
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
void DrawAreaWidget::openold(){
    if (!openasadd){
        if (!maybeSave()) return;
    }



    QStringList files;
    QFileDialog dlg(this,"Open file");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setNameFilter("INI (*.ini   *.inf)");
    dlg.exec();
    if (dlg.result()==QDialog::Accepted)
        files = dlg.selectedFiles();
    else
        return;
    if(files.isEmpty())
        return;
    if (!openasadd) init();
    QFile openFile(files.at(0));
    openFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&openFile);
    QString line;
    while(!in.atEnd())
    {
        line=in.readLine();
        add(line);
    }
}

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
    if (e.hasAttribute("red")){
        int r,g,b;
        r=e.attribute("red").toInt();
        g=e.attribute("green").toInt();
        b=e.attribute("blue").toInt();
        backcolor=QColor(r,g,b);
    }

    Combo* tmp=new Combo;
    tmp->setShapes(root);
    //shapes=tmp->shapes;
    shapes +=tmp->shapes;
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

//    openFile.open(QFile::ReadOnly | QFile::Text);
//    QTextStream in(&openFile);
//    QString line;
//    while(!in.atEnd())
//    {
//        line=in.readLine();
//        add(line);
//    }
}


void DrawAreaWidget::add(QString line){
//    int a=line.indexOf("(",0);
//    QString shapeName=line.mid(0,a);
//    //switch (shapeName)
//    if (shapeName=="Curve"){
//        currentShape = new Curve;
//        //
//        //

//    }
//    if (shapeName=="Circle"){
//        currentShape = new Circle;
//        //
//        //

//    }
//    if (shapeName=="Ellipse"){
//        currentShape = new Ellipse;
//        //
//        //

//    }
//    if (shapeName=="Polygon"){
//        currentShape = new Polygon;
//        //
//        //

//    }
//    if (shapeName=="Polyline"){
//        currentShape = new Polyline;
//        //
//        //

//    }
//    if (shapeName=="Rect"){
//        currentShape = new Rect;
//        //
//        //

//    }
//    if (shapeName=="Text"){
//        currentShape = new Text;
//        //
//        //

//    }
//    if (currentShape==NULL) currentShape=new Curve;
//    shapes.append(currentShape);
//    currentShape->fromline(line);
//    expand();update();

}


void DrawAreaWidget::save(){
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


    //saveFile.close();
    saved=true;

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
    foreach(GeneralShape* sp,shapes){
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
    foreach(GeneralShape* sp,shapes){
        sp->setPen(QPen(Qt::black));
    }

    foreach(GeneralShape* sp,pickedShapes){
        sp->setPen(QPen(Qt::red,3));
    }
    foreach(GeneralShape* sp,shapes){
        sp->draw(painter,zoomRatio);
    }

    foreach(GeneralShape* sp,pickedShapes){
        sp->drawClosure(painter,zoomRatio);
    }

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
        delete menu;
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
            if (pickedShapes.at(0)->name=="Text"){

                Text *pm = dynamic_cast< Text* >( pickedShapes.at(0) );
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


            QColorDialog* gtd=new QColorDialog;
            //qDebug()<<pickedShapes.at(0)->brush;
            if (pickedShapes.at(0)->brush.style()!=Qt::NoBrush){
                gtd->setCurrentColor(pickedShapes.at(0)->brush.color());
            }
            if (gtd->exec()==QDialog::Accepted){
                pickedShapes.at(0)->setBrush(gtd->selectedColor() );
            }
            //pickedShapes.at(0)->setBrush(gtd->getColor());
            expand();update();


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
        QColorDialog* gtd=new QColorDialog;
        //qDebug()<<pickedShapes.at(0)->brush;
        if (pickedShapes.at(0)->brush.style()!=Qt::NoBrush){
            gtd->setCurrentColor(pickedShapes.at(0)->brush.color());
        }
        if (gtd->exec()==QDialog::Accepted){
            pickedShapes.at(0)->setBrush(gtd->selectedColor() );
        }
        //pickedShapes.at(0)->setBrush(gtd->getColor());
        expand();update();
    }
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
        if (currentCategory==CurveCategory) currentShape = new Curve;
        else currentShape = new CloseCurve;
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
//            foreach(GeneralShape* sp,shapes){
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
                break;

            }
            if (isScaleHandlePoint(realPoint,pickedShapes.at(0))){//scale pickedShape
                qDebug()<<"scale pickedShape";
                currentMouseHanded=ScalePoint;
                //startPoint=realPoint;
                startPoint=pickedShapes.at(0)->scaleHandlePoint();
                pickedShapestartsx=pickedShapes.at(0)->sx;
                pickedShapestartsy=pickedShapes.at(0)->sy;
                break;
            }
            currentMouseHanded=None;

            qDebug()<<"nothing pickedShape";
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
//        foreach(GeneralShape* sp,shapes){
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
            pickRect=new Rect;
            pickRect->addPoint(realPoint);
            pickRect->addPoint(realPoint);

        }
        //else //qDebug()<<"Nothing";
        break;
    }
    case CircleCategory:{
        setMouseTracking(false);
        currentShape = new Circle;

        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }

    case RectCategory:{
        setMouseTracking(false);
        currentShape = new Rect;

        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }
    case EllipseCategory:{
        setMouseTracking(false);
        currentShape = new Ellipse;

        isLeftButtonPressed = true;

        currentShape->addPoint(realPoint);
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        break;
    }
    case TextCategory:{
        setMouseTracking(false);
        currentShape = new Text;

        isLeftButtonPressed = true;



        break;
    }
    }


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
        if (currentShape->isEmpty()) shapes.removeLast();
        currentShape = NULL;
        break;
    }
    case PolylineCategory:
    {
        setMouseTracking(true);
        if (currentShape==NULL){
            currentShape = new Polyline;
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
            if (currentShape->isEmpty()) shapes.removeLast();
            currentShape = NULL;
            //currentCategory = PolylineCategory;
            isLeftButtonPressed = false;
        }
        break;
    }
    case PolygonCategory:
    {
        setMouseTracking(true);
        if (currentShape==NULL){
            currentShape = new Polygon;
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
            if (currentShape->isEmpty()) shapes.removeLast();
            currentShape = NULL;
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
            pickRect=NULL;
            //qDebug()<<"";
            update();
            endPoint=realPoint;
            pickedShapes=pickShape(startPoint,endPoint);
            if (pickedShapes.size()>0) {

                update();
            }
            return;
        }
        endPoint=realPoint;
        //qDebug()<<endPoint-startPoint;
        startPoint=endPoint;
        pickedShapes.at(0)->drag(endPoint-startPoint);

        update();
        break;
    }
    case CircleCategory:
    {
        currentShape->removeLastPoint();
        currentShape->addPoint(realPoint);
        isLeftButtonPressed = false;
        currentShape = NULL;
        update();
        break;
    }
    case TextCategory:{//input qstring
        currentShape->addPoint(realPoint);
        shapes.append(currentShape);

        GetTextDialog* gtd=new GetTextDialog;
//        QString currentText="hello world!";
        gtd->setText("hello world!");

        if (gtd->exec()==QDialog::Accepted){
            Text *pm = dynamic_cast< Text* >( currentShape );
            if ( pm ) {
                //pm->setText(currentText);
                pm->setText(gtd->text);
            }
            //currentShape->setText(currentText );
        }
        currentShape = NULL;
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
        currentShape = NULL;
        update();
        break;
    }
    }
}
void DrawAreaWidget::keyPressEvent(QKeyEvent *event){
    if (event->key()==Qt::Key_Delete){

        if (pickedShapes.size()>0){
            foreach(GeneralShape* sp,pickedShapes){
                del(sp);
            }
            pickedShapes.clear();
            update();
        }
    }
    if (event->key()==Qt::Key_W){
        //qDebug()<<"Up pressed";
      pickedMove(0,-1/zoomRatio);
    }
    if (event->key()==Qt::Key_S){
        //qDebug()<<"Up pressed";
      pickedMove(0,1/zoomRatio);
    }
    if (event->key()==Qt::Key_A){
        //qDebug()<<"Up pressed";
      pickedMove(-1/zoomRatio,0);
    }
    if (event->key()==Qt::Key_D){
        //qDebug()<<"Up pressed";
      pickedMove(1/zoomRatio,0);
    }
}

void DrawAreaWidget::pickedMove(qreal x, qreal y){
    //qDebug()<<"Up pressed";
    if (pickedShapes.size()==1){
        pickedShapes.at(0)->drag(QPointF(x,y));
        expand();update();
    }
    if (pickedShapes.size()>0){
        foreach(GeneralShape* sp,pickedShapes){
            sp->drag(QPointF(x,y));
        }
        expand();update();
    }
}

void DrawAreaWidget::del(GeneralShape * sp){
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
            //expand();
            update();
            break;
        }
        if (currentMouseHanded==ScalePoint){//scale,NEED REWRITE!!!
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

            //expand();
            update();
            return;
        }
        if (pickedShapes.size()>0){
            startPoint=endPoint;
            endPoint=realPoint;
            //pickedShapes->drag(endPoint-startPoint);
            foreach(GeneralShape* sp,pickedShapes){
                sp->drag(endPoint-startPoint);
            }
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
}
void DrawAreaWidget::finishcurrentShape(){

    currentShape = NULL;
    update();
}
QList<GeneralShape *> DrawAreaWidget::pickShape(QPointF point){
//    foreach(GeneralShape* sp,shapes){//这种方式选取顺序为从底下往上面。
//        if (sp->minDistance(point)*zoomRatio<4) return sp;
//    }
//    return NULL;
    QList<GeneralShape *> tmp;
    for(int i=shapes.size()-1;i>=0;i--){//这种方式选取顺序为从上面往底下。
        if (shapes.at(i)->minDistance(point)*zoomRatio<4) {
            tmp.append(shapes.at(i)) ;
            break;
        }
    }
    return tmp;
}
QList<GeneralShape *> DrawAreaWidget::pickShape(QPointF p0,QPointF p1){
    QList<GeneralShape *> tmp;
    foreach(GeneralShape* sp,shapes){
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
bool DrawAreaWidget::isScaleHandlePoint(QPointF realPoint,GeneralShape* pickedShape){
    QPointF handlePoint=pickedShapes.at(0)->scaleHandlePoint();
    //qDebug()<<handlePoint;
    double x=handlePoint.x();
    double y=handlePoint.y();
    //qDebug()<<x-realPoint.x()<<y-realPoint.y();
    if (abs(x-realPoint.x())*zoomRatio<3 && abs(y-realPoint.y())*zoomRatio<3) return true;
    else return false;
}

bool DrawAreaWidget::isRotationHandlePoint(QPointF realPoint,GeneralShape* pickedShape){
    QPointF handlePoint=pickedShapes.at(0)->rotationHandlePoint();
    //qDebug()<<handlePoint;
    double sy=pickedShapes.at(0)->getsy();
    double x=handlePoint.x()+sy/abs(sy)*lenthOfRotationHandleLine/zoomRatio*sin(pickedShapes.at(0)->rotationangle()/180*M_PI);
    double y=handlePoint.y()-sy/abs(sy)*lenthOfRotationHandleLine/zoomRatio*cos(pickedShapes.at(0)->rotationangle()/180*M_PI);
    //qDebug()<<x-realPoint.x()<<y-realPoint.y();
    if (abs(x-realPoint.x())*zoomRatio<3 && abs(y-realPoint.y())*zoomRatio<3) return true;
    else return false;
}

void DrawAreaWidget::expand(){
    foreach(GeneralShape* sp,shapes){
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
//void DrawAreaWidget::onscrollDone(){
//    isScrolling=false;
//}
bool DrawAreaWidget::inRange(QPointF Point, QList<GeneralShape *> sps){
    foreach(GeneralShape* sp,sps){
        if (Point.x()>sp->left && Point.x()<sp->right && Point.y()>sp->top && Point.y()<sp->bottom) return true;

    }

    return false;

}
void DrawAreaWidget::test(){
//    pickedShapes.at(0)->zoom(2);
//    update();
}
