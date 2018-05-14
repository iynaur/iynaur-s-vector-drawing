#pragma once
#include "abstractaction.h"
class ACTIONS_EXPORT KeyPointMoveAction :
	public AbstractAction
{
public:
	KeyPointMoveAction();
	~KeyPointMoveAction();
	QPointF dpoint;
	int m_id;
	//QBrush oldbrush,newbrush;
	virtual QString name();
	void reallyDo();
	void undo();
	virtual void setPickedShapes();
	void moveKeyPoint(int id, QPointF dp);//p is the last point, not dp
};

