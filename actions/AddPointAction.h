#pragma once
#include "abstractaction.h"
class ACTIONS_EXPORT AddPointAction :
	public AbstractAction
{
public:
	AddPointAction();
	virtual ~AddPointAction();

	QPointF newPoint;
	int m_id;
	//QBrush oldbrush,newbrush;
	virtual QString name();
	void reallyDo();
	void undo();
};

