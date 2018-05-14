#include "KeyPointMoveAction.h"
#include "polyline.h"


KeyPointMoveAction::KeyPointMoveAction()
{
}


KeyPointMoveAction::~KeyPointMoveAction()
{
}


QString KeyPointMoveAction::name()
{
	return "MovePoint";
}

void KeyPointMoveAction::reallyDo()
{
	moveKeyPoint(m_id, dpoint);
	//shapes[0]->points[m_id] += dpoint;
	//shapes[0]->updateRange();
	pickedShapes = shapes;
}

void KeyPointMoveAction::undo()
{
	moveKeyPoint(m_id, -dpoint);
	pickedShapes = shapes;
}

void KeyPointMoveAction::setPickedShapes()
{
	pickedShapes = shapes;
}

void KeyPointMoveAction::moveKeyPoint(int id, QPointF dp)
{
	auto pl = dynamic_pointer_cast<Polyline>(shapes[0]);
	if(pl)pl->moveKeyPoint( id,  dp);
}
