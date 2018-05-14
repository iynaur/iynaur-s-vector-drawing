#pragma once
#include "shapes.h"
#include <memory>
#include "AbstractAction.h"
#include "GeneralShape.h"
class DrawAreaWidget;
class dataserver : public QObject
{
	Q_OBJECT
public:
	explicit dataserver(DrawAreaWidget* fa);
	void init(DrawAreaWidget* fa);
	void undoStack_push(AbstractAction* act);
	void shapes_append(shared_ptr<GeneralShape> currentShape);

private:
	QUndoStack* undoStack;
	QList<shared_ptr<GeneralShape> >* m_pShapes;

signals:
	void update();

};