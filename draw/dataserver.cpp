#include "dataserver.h"
#include "DrawAreaWidget.h"
using namespace std;
dataserver::dataserver(DrawAreaWidget* fa) :
	QObject()
{
	init(fa);
}

void dataserver::undoStack_push(AbstractAction * act)
{
	undoStack->push(act);
	emit update();
}

void dataserver::shapes_append(shared_ptr<GeneralShape> currentShape)
{
	m_pShapes->append(currentShape);
}

void dataserver::init(DrawAreaWidget* fa)
{
	m_pShapes = &(fa->shapes);
	undoStack = &(fa->undoStack);
}
