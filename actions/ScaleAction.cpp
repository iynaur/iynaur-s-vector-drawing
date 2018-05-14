#include "ScaleAction.h"



ScaleAction::ScaleAction()
{
}

void ScaleAction::setScaleAction(int id, QPointF from, QPointF to)
{
	m_id = id;
	m_to = to;
	m_from = from;
}


ScaleAction::~ScaleAction()
{
}


QString ScaleAction::name()
{
	return "Scale";
}

void ScaleAction::reallyDo()
{
	shapes[0]->scale(m_id, m_to);
	setPickedShapes();
}

void ScaleAction::undo()
{
	shapes[0]->scale(m_id, m_from);
	setPickedShapes();
}

void ScaleAction::setPickedShapes()
{
	pickedShapes = shapes;
}
