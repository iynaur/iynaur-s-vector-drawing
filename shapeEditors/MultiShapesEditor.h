#pragma once
#include "ShapeEditorBase.h"
class MultiShapesEditor :
	public ShapeEditorBase
{
public:
	MultiShapesEditor();
	~MultiShapesEditor();
	virtual bool mouseDown(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseMove(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseUp(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
};

