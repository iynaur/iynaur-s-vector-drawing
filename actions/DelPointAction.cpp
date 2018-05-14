#include "DelPointAction.h"
#include "polyline.h"


DelPointAction::DelPointAction()
{
}


DelPointAction::~DelPointAction()
{
}


QString DelPointAction::name()
{
	return "DelPoint";
}

void DelPointAction::reallyDo()
{
	auto pl = dynamic_pointer_cast<Polyline>(shapes[0]);
	if (pl) pl->delPoint(m_id);
	//shapes[0]->points.removeAt(m_id);
	//shapes[0]->updateRange();
	pickedShapes = shapes;
}

void DelPointAction::undo()
{
	auto pl = dynamic_pointer_cast<Polyline>(shapes[0]);
	if (pl) pl->addPoint(m_id, newPoint);
	//shapes[0]->points.insert(m_id, newPoint);
	//shapes[0]->updateRange();
	pickedShapes = shapes;
}

