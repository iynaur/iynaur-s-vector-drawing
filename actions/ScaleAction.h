#pragma once
#include "abstractaction.h"
class ACTIONS_EXPORT ScaleAction :
	public AbstractAction
{
public:
	ScaleAction();
	void setScaleAction(int id, QPointF from, QPointF to);
	virtual ~ScaleAction();

	virtual QString name();
	void reallyDo();
	void undo();
	virtual void setPickedShapes();

private:
	QPointF m_to, m_from;
	int m_id;

};

