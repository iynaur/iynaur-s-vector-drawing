#pragma once
#include "ICallBack.h"
class DrawAreaWidget;
class CallBack :
	public ICallBack
{
public:
	CallBack();
	virtual ~CallBack();
	void call(std::string msg);
	DrawAreaWidget* m_canvas;
};

