#include "facetool.h"
FaceTool::FaceTool()
{

}
FaceTool::~FaceTool()
{
}
void FaceTool::mouseDown(QPointF p, bool bLeft){
	if (m_Shape != nullptr) return;
	p1 = p2 = p;
    shared_ptr<Combo> com = shared_ptr<Combo>(new Combo());
    e=shared_ptr<Ellipse>(new Ellipse());
    m=shared_ptr<Polyline>(new Polyline());
    m1=shared_ptr<Polyline>(new Polyline());
    m2=shared_ptr<Polyline>(new Polyline());
    com->shapes.append(e);

    com->shapes.append(m);

    com->shapes.append(m1);

    com->shapes.append(m2);
    construct();
	m_Shape = static_pointer_cast<GeneralShape>(com);
	this->publishShapeCreated();
    //break;

}
void FaceTool::mouseMove(QPointF p, bool bLeft){
	if (m_Shape == nullptr) return;
	p2 = p;
    construct();
}
void FaceTool::mouseUp(QPointF p, bool bLeft){
	if (m_Shape == nullptr) return;
	p2 = p;
    construct();
	m_Shape->updateRange();
	this->publishShapeFinished();
	m_Shape = nullptr;

}
void FaceTool::construct(){
    e->points.clear();
    m->points.clear();
    m1->points.clear();
    //m2->points.clear();
    qreal top,bottom,left,right,w,h;
    top=p1.y();
    bottom=p2.y();
    left=p1.x();
    right=p2.x();
    w=right-left;
    h=bottom-top;
    e->appendPoint(p1);
    e->appendPoint(p2);
    m->appendPoint(QPointF(left+w/4,bottom-h/4));
    m->appendPoint(QPointF(left+w/2,bottom-h/8));
    m->appendPoint(QPointF(right-w/4,bottom-h/4));
    m1->appendPoint(QPointF(left+w/4,bottom-h*5/8));
    m1->appendPoint(QPointF(left+w*5/16,top+h/4));
    m1->appendPoint(QPointF(left+w*3/8,bottom-h*5/8));
    *m2=*(dynamic_pointer_cast<Polyline>(m1->copyPaste()));//对m2赋值之后，m2的改变不影响原来com->shapes.append(m2);
    m2->drag(QPointF(w*3/8,0));
}
