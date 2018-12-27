#pragma once
#include "ICallback.h"
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

