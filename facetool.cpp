#include "facetool.h"
#include"drawareawidget.h"
FaceTool::FaceTool()
{


}
void FaceTool::mousePress(QPointF p){
    canvas->finishcurrentShape();
    shared_ptr<Combo> com = shared_ptr<Combo>(new Combo());
    canvas->currentShape = com;

    canvas->isLeftMouseButtonPressed = true;

    canvas->currentShape->mousePress(p);
    e=shared_ptr<Ellipse>(new Ellipse());
    m=shared_ptr<Polyline>(new Polyline());
    m1=shared_ptr<Polyline>(new Polyline());
    m2=shared_ptr<Polyline>(new Polyline());
    com->shapes.append(e);

    com->shapes.append(m);

    com->shapes.append(m1);

    com->shapes.append(m2);
    construct();
    canvas->shapes.append(canvas->currentShape);

    //break;

}
void FaceTool::mouseMove(QPointF p){
    if (canvas->currentShape!=NULL //&& isLeftMouseButtonPressed
            ){
        canvas->currentShape->mouseMove(p);
        construct();
        //update();
    }
}
void FaceTool::mouseRelease(QPointF p){
    if (canvas->currentShape==0) return;
    canvas->currentShape->removeLastPoint();
    canvas->currentShape->addPoint(p);
    construct();
    canvas->isLeftMouseButtonPressed = false;

    canvas->finishcurrentShape();

}
void FaceTool::construct(){
    e->points.clear();
    m->points.clear();
    m1->points.clear();
    //m2->points.clear();
    qreal top,bottom,left,right,w,h;
    top=canvas->currentShape->points.at(0).y();
    bottom=canvas->currentShape->points.at(1).y();
    left=canvas->currentShape->points.at(0).x();
    right=canvas->currentShape->points.at(1).x();
    w=right-left;
    h=bottom-top;
    e->addPoint(canvas->currentShape->points.at(0));
    e->addPoint(canvas->currentShape->points.at(1));
    m->addPoint(QPointF(left+w/4,bottom-h/4));
    m->addPoint(QPointF(left+w/2,bottom-h/8));
    m->addPoint(QPointF(right-w/4,bottom-h/4));
    m1->addPoint(QPointF(left+w/4,bottom-h*5/8));
    m1->addPoint(QPointF(left+w*5/16,top+h/4));
    m1->addPoint(QPointF(left+w*3/8,bottom-h*5/8));
    *m2=*(dynamic_pointer_cast<Polyline>(m1->copyPaste()));//对m2赋值之后，m2的改变不影响原来com->shapes.append(m2);
    m2->drag(QPointF(w*3/8,0));
}
