#include "stable.h"
#include "CallBack.h"
#include "drawareawidget.h"

CallBack::CallBack()
{
}


CallBack::~CallBack()
{
}

void CallBack::call(std::string msg)
{
	if (msg == "shapeCreated") m_canvas->creatShape();
	else if (msg == "shapeFinished") m_canvas->finishShape();
	else if (msg == "editFinished") m_canvas->editShape();
}
