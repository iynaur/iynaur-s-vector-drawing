#include "AddPointAction.h"
#include "polyline.h"


AddPointAction::AddPointAction()
{
}


AddPointAction::~AddPointAction()
{
}


QString AddPointAction::name()
{
	return "AddPoint";
}

void AddPointAction::reallyDo()
{
	auto pl = dynamic_pointer_cast<Polyline>(shapes[0]);
	if (pl) pl->addPoint(m_id, newPoint);
	//shapes[0]->points.insert(m_id, newPoint);
	//shapes[0]->updateRange();
	pickedShapes = shapes;
}

void AddPointAction::undo()
{
	auto pl = dynamic_pointer_cast<Polyline>(shapes[0]);
	if (pl) pl->delPoint(m_id);
	//shapes[0]->points.removeAt(m_id);
	//shapes[0]->updateRange();
	pickedShapes = shapes;
}


