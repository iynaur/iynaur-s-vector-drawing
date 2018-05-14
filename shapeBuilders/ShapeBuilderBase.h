#pragma once
#include "IShapeBuilder.h"
#include "ShapeBuilders_global.h"

class ShapeBuilderBase :
	public IShapeBuilder
{
public:
	ShapeBuilderBase();
	virtual ~ShapeBuilderBase();
	virtual void publishShapeCreated();
	virtual void publishShapeFinished();
	void setCallBack(ICallBack* callBack);
	virtual shared_ptr<GeneralShape> shape();

	shared_ptr<GeneralShape> m_Shape;
	ICallBack* m_callBack;
};




