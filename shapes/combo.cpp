#include "combo.h"
#include "arrow.h"

Combo::Combo()
{
    //name="Combo";
    minx=miny=maxx=maxy=0;
    Rotationangle=0;
    sx=1;
    sy=1;
    QList<shared_ptr<GeneralShape>> list;
    shapes=list;
    QVector<QPointF> vector;
    points=vector;
    //updatedrawcombo();
}

Combo::~Combo(){
    foreach(shared_ptr<GeneralShape> sp,shapes){
        //delete sp;
    }
    //qDebug()<<"delete";
}

QString Combo::name(){
    return "Combo";
}
 shared_ptr<GeneralShape> Combo::copyPaste(){
    shared_ptr<Combo> tmp=shared_ptr<Combo>(new Combo);
    foreach (shared_ptr<GeneralShape> sp, shapes) {
        tmp->shapes.append(sp->copyPaste());
        tmp->shapes.last()->updateRange();
    }
    tmp->points=points;
    //tmp->name=name;
    tmp->pen=pen;
    tmp->Rotationangle=Rotationangle;
    tmp->sx=sx;
    tmp->sy=sy;
    tmp->brush=brush;

    tmp->updateRange();//这一步不能少！！！！！！--
    //tmp->updatedrawcombo();
    return static_pointer_cast<GeneralShape>(tmp);
}
void Combo::setShapesFromNode(QDomNode node){
    while(!node.isNull())  //如果节点不空--
    {
        if(node.isElement()) //如果节点是元素---
        {
            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多--

            shared_ptr<GeneralShape> currentShape;



            QDomNode n=e.namedItem("name");

            currentShape=NULL;
            if (n.toElement().text()=="Curve"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Curve>(new Curve));
            }
            if (n.toElement().text()=="CloseCurve"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<CloseCurve>(new CloseCurve));
            }
            if (n.toElement().text()=="Circle"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Circle>(new Circle));
            }
            if (n.toElement().text()=="Ellipse"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Ellipse>(new Ellipse));
            }
            if (n.toElement().text()=="arrow"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Arrow>(new Arrow));
            }
            if (n.toElement().text()=="Polygon"
                    //|| n.toElement().text()=="Poly"
                    ){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Polygon>(new Polygon));
            }
            if (n.toElement().text()=="Polyline"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Polyline>(new Polyline));
            }
            if (n.toElement().text()=="Rect"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Rect>(new Rect));
            }
            if (n.toElement().text()=="Text"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Text>(new Text));
            }
            if (n.toElement().text()=="Combo"){
                currentShape = static_pointer_cast<GeneralShape>(shared_ptr<Combo>(new Combo));
            }
            if (currentShape!=NULL) {



            //if (n.nodeName()=="points")

            n=e.namedItem("points");
            QString line=n.toElement().text();

            currentShape->fromline(line);

            //if (n.nodeName()=="Rotationangle")

            n=e.namedItem("Rotationangle");

            currentShape->Rotationangle=n.toElement().text().toDouble();


            //if (n.nodeName()=="mytext")

            n=e.namedItem("mytext");
            shared_ptr<Text >pm = dynamic_pointer_cast< Text >( currentShape );
            if ( pm ){
                pm->setText(n.toElement().text());

            }


            //if (n.nodeName()=="myfont")

            n=e.namedItem("myfont");
            pm = dynamic_pointer_cast< Text >( currentShape );
            if ( pm ){
                QFont font;
                font.setPointSize(n.toElement().attribute("pointSize").toInt());
                pm->setFont(font);
            }


            //if (n.nodeName()=="shapes")

            n=e.namedItem("shapes");

            shared_ptr<Combo >cm = dynamic_pointer_cast< Combo >( currentShape );
            if ( cm ){
                cm->setShapes(n.toElement());

            }


            //if (n.nodeName()=="brush")

            n=e.namedItem("brush");

            int r,g,b,brushstyle;
            r=n.toElement().attribute("red").toInt();
            g=n.toElement().attribute("green").toInt();
            b=n.toElement().attribute("blue").toInt();




            brushstyle=n.toElement().attribute("brushstyle").toInt();
            Qt::BrushStyle castStyle = (Qt::BrushStyle)brushstyle;
            //currentShape->brush.setColor(QColor(r,g,b));//this code has no effect,why?
            //currentShape->setBrush(QColor(r,g,b),Qt::BrushStyle(brushstyle));
            QBrush brush=QBrush(QColor(r,g,b),castStyle);
            currentShape->setBrush(brush);

            n=e.namedItem("pen");

            //int r,g,b,brushstyle;
            r=n.toElement().attribute("red").toInt();
            g=n.toElement().attribute("green").toInt();
            b=n.toElement().attribute("blue").toInt();
            currentShape->setPen(QPen(QColor(r,g,b)));


            currentShape->setsx ( e.attribute("sx").toDouble()!=0 ? e.attribute("sx").toDouble() : 1);
            currentShape->setsy ( e.attribute("sy").toDouble()!=0 ? e.attribute("sy").toDouble() : 1);
            //qDebug()<<currentShape->sx;
            currentShape->updateRange();
            if (!currentShape->isEmpty()) {
                shapes.append(currentShape);
                //qDebug()<<currentShape->name()<<"added";
            }
            }
        }
        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    }
    updateRange();
}

void Combo::setShapes(QDomElement shapesElement){

    QDomNode node=shapesElement.firstChild(); //获得第一个子节点
    setShapesFromNode(node);
}
//QDomDocument Combo::toDocument(){
//    QDomDocument doc;
//    //QDomElement shapesElement=doc.createElement("shapes");

//    doc.appendChild(toElement());
//    return doc;
//}
QDomElement Combo::toElement(shared_ptr<GeneralShape>sp){
    QDomDocument doc;
    QDomElement shape=doc.createElement("shape");
    shape.setAttribute("sx",sp->getsx()); //方式一：创建属性  其中键值对的值可以是各种类型
    shape.setAttribute("sy",sp->getsy());

    QDomText text; //设置括号标签中间的值

    QDomElement name=doc.createElement("name"); //创建子元素
    text=doc.createTextNode(sp->name());
    shape.appendChild(name);
    name.appendChild(text);

    QDomElement points=doc.createElement("points"); //创建子元素
    text=doc.createTextNode(sp->qStringFromPoints());
    shape.appendChild(points);
    points.appendChild(text);

    QDomElement Rotationangle=doc.createElement("Rotationangle"); //创建子元素
    text=doc.createTextNode(QString::number(sp->rotationangle()) );
    shape.appendChild(Rotationangle);
    Rotationangle.appendChild(text);

    QDomElement brush=doc.createElement("brush"); //创建子元素
    brush.setAttribute("red",sp->brush.color().red());
    brush.setAttribute("green",sp->brush.color().green());
    brush.setAttribute("blue",sp->brush.color().blue());
    brush.setAttribute("brushstyle",sp->brush.style());
    shape.appendChild(brush);

    QDomElement pen=doc.createElement("pen"); //创建子元素
    pen.setAttribute("red",sp->pen.color().red());
    pen.setAttribute("green",sp->pen.color().green());
    pen.setAttribute("blue",sp->pen.color().blue());
    shape.appendChild(pen);


    if (sp->name()=="Text"){
        QDomElement mytext=doc.createElement("mytext"); //创建子元素--
        QDomElement myfont=doc.createElement("myfont");

        shared_ptr<Text >pm = dynamic_pointer_cast< Text >( sp );
        if ( pm ){
        text=doc.createTextNode(pm->text());
        shape.appendChild(mytext);
        mytext.appendChild(text);

        myfont.setAttribute("pointSize",pm->myfont.pointSize());
        shape.appendChild(myfont);

        }

    }
    if (sp->name()=="Combo"){
        shared_ptr<Combo >pm = dynamic_pointer_cast< Combo >( sp );
        if ( pm ){
            QDomElement shapes=pm->toElement();
        shape.appendChild(shapes);
        }

    }
    return shape;
}
QDomDocument Combo::toDocument(){
    QDomDocument doc;
    QDomElement shapesElement=doc.createElement("shapes");//just to avoid "Calling appendChild() on a null node does nothing."
    foreach(shared_ptr<GeneralShape>sp, shapes){


        doc.appendChild(toElement(sp));
//        out << shape->qStringFromThis();
//        out<<endl;
    }
    return doc;
}

QDomElement Combo::toElement(){
    QDomDocument doc;
    QDomElement shapesElement=doc.createElement("shapes");

    doc.appendChild(shapesElement);




    foreach(shared_ptr<GeneralShape>sp, shapes){


        shapesElement.appendChild(toElement(sp));
//        out << shape->qStringFromThis();
//        out<<endl;
    }
    return shapesElement;
}



//QString Combo:: qStringFromThis(){
//    return "";
//}

QString Combo:: qStringFromPoints(){
    return "";
}


void Combo:: draw(QPainter &painter,qreal zoomRatio){//性能
    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;

    painter.translate((left+right)/2*zoomRatio, (top+bottom)/2*zoomRatio);
    painter.rotate( Rotationangle );
    shared_ptr<Combo> drawcombo=static_pointer_cast<Combo>(copyPaste());

    drawcombo->drag(QPointF(-(left+right)/2,-(top+bottom)/2));
    foreach (shared_ptr<GeneralShape> sp, drawcombo->shapes) {
        //shared_ptr<GeneralShape>tmp2=sp->copyPaste();
//        if (sp->name()!="Text"){
        sp->zoom(sx, sy);
        sp->draw(painter,zoomRatio);

    }
    //delete drawcombo;
    //drawcombo->drag(-QPointF(-(left+right)/2,-(top+bottom)/2));
    //zoom(1/sx);
    //drag(QPointF((left+right)/2,(top+bottom)/2));
    painter.rotate( -Rotationangle );
    painter.translate(-((left+right)/2)*zoomRatio, -((top+bottom)/2)*zoomRatio);


}



void Combo:: zoom(qreal zoomratio){
    foreach (shared_ptr<GeneralShape> sp, shapes) {
        sp->zoom(zoomratio);
    }

    updateRange();
    //updatedrawcombo();
}

double Combo:: minDistance(QPointF point){
    double left=minx;
    double right=maxx;
    double top=miny;
    double bottom=maxy;

    point=point-QPointF((left+right)/2,(top+bottom)/2);
    point=rotated(point,Rotationangle/180*M_PI);

    drag(QPointF(-(left+right)/2,-(top+bottom)/2));
    double minD=MAX;
    foreach (shared_ptr<GeneralShape> sp, shapes) {
        shared_ptr<GeneralShape> tmp=sp->copyPaste();
        tmp->zoom(sx, sy);
        minD=min(minD,tmp->minDistance(point));
        //delete tmp;
    }
    drag(QPointF((left+right)/2,(top+bottom)/2));


    return minD;
}



void Combo:: zoom(qreal zx,qreal zy)
{
    foreach (shared_ptr<GeneralShape> sp, shapes) {
        sp->zoom(zx, zy);
    }
    updateRange();
}



void Combo:: updateRange() {
    if (shapes.size()==0) return;
    minx=shapes.at(0)->left;
    maxx=shapes.at(0)->right;
    miny=shapes.at(0)->top;
    maxy=shapes.at(0)->bottom;
    for (int i=1;i<shapes.size();i++){

        minx=min(shapes.at(i)->left,minx);
        miny=min(shapes.at(i)->top,miny);
        maxx=max(shapes.at(i)->right,maxx);
        maxy=max(shapes.at(i)->bottom,maxy);
    }
    updateBand();
}

void Combo:: drag(QPointF point)
{
    for (int i=0;i<shapes.size();i++){
        shapes.at(i)->drag(point);
    }
    updateRange();
}
bool Combo:: isEmpty(){
    return sx == 0 && sy == 0;
}

void Combo::setRotationangle(qreal angle){
    Rotationangle=angle;
}
//void Combo:: addPoint(QPointF point){

//}

//void Combo:: removeLastPoint(){

//}

void Combo:: setPen(const QPen &pen){
//    for (int i=0;i<shapes.size();i++){
//        shapes.at(i)->setPen(pen);
//    }
}

void Combo:: setBrush(const QBrush &brush){

}
void Combo:: setsx(double x){
    sx=x;

    //updatedrawcombo();
}
void Combo:: setsy(double y){
    sy=y;
    //do nothing
}
double Combo:: getsy(){
    return sy;
}

void Combo::fitToPoints(QPointF startPoint, QPointF endPoint, Combo *combo)
{
    assert(combo->getsy() == 1);
    assert(combo->getsy() == 1);
    combo->updateRange();
    auto center_combo_x = (combo->minx + combo->maxx)/2;
    auto center_combo_y = (combo->miny + combo->maxy)/2;
    auto center_pos = (startPoint + endPoint) / 2;
    combo->drag(center_pos - QPointF(center_combo_x, center_combo_y));
    combo->setsx( (-startPoint + endPoint).x() / (-combo->minx + combo->maxx));
    combo->setsy( (-startPoint + endPoint).y() / (-combo->miny + combo->maxy));
}
