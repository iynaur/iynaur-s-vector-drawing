#pragma once
#include "abstractaction.h"
class ACTIONS_EXPORT DelPointAction :
	public AbstractAction
{
public:
	DelPointAction();
	virtual ~DelPointAction();

	QPointF newPoint;//visual point
	int m_id;
	//QBrush oldbrush,newbrush;
	virtual QString name();
	void reallyDo();
	void undo();
};

