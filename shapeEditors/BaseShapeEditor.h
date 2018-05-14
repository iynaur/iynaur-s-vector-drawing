#pragma once
#include "ShapeEditorBase.h"
class BaseShapeEditor :
	public ShapeEditorBase
{
public:
	BaseShapeEditor();
	~BaseShapeEditor();

	virtual bool mouseDown(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseMove(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseUp(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	void virtual drawControlPoints(QPainter &painter, qreal zoomRatio);



};

