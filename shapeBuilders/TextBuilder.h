#pragma once
#include "ShapeBuilderBase.h"
class TextBuilder :
	public ShapeBuilderBase
{
public:
	TextBuilder();
	virtual ~TextBuilder();

	virtual void mouseDown(QPointF p, bool bLeft = true);
	virtual void mouseMove(QPointF p, bool bLeft = true);
	virtual void mouseUp(QPointF p, bool bLeft = true);
};

