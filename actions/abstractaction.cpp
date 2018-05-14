#include "abstractaction.h"


AbstractAction::AbstractAction()
{
	m_bHasDone = false;
}
AbstractAction::~AbstractAction(){
//    foreach (GeneralShape*sp,shapes){
//        delete sp;
//    }
}

void AbstractAction::redo()
{
	if (!m_bHasDone) reallyDo();
	setPickedShapes();
	m_bHasDone = false;
}

void AbstractAction::setPickedShapes()
{
}
