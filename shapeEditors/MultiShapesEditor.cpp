#include "MultiShapesEditor.h"
#include "editaction.h"


MultiShapesEditor::MultiShapesEditor()
{
}


MultiShapesEditor::~MultiShapesEditor()
{
}



bool MultiShapesEditor::mouseDown(QPointF p, bool bLeft, QMouseEvent * event)
{
	bool bIn = false;
	for (auto sp : m_shapes) {
		if (sp->minDistance(p)*m_zoomRatio <= 4) {
			bIn = true;
			break;
		}
	}
	if (!bIn) return 0;
	currentMouseHanded = None;

	endPoint = startPoint = p;
	return 1;
}

void MultiShapesEditor::mouseMove(QPointF p, bool bLeft, QMouseEvent * event)
{
	for (auto sp : m_shapes) {
		sp->drag(p - endPoint);
	}
	endPoint = p;
	return;
}

void MultiShapesEditor::mouseUp(QPointF realPoint, bool bLeft, QMouseEvent * event)
{
		EditAction* action = new EditAction();
		action->shapes=m_shapes;
		action->dpoint = endPoint - startPoint;
		//AbstractAction* tmp=static_cast<AbstractAction*>(action);
		if (action->dpoint != QPointF(0, 0)) {
			//tmp->pickedShapes=&pickedShapes;
			action->m_bHasDone = true;
			m_action = action;
			publishEditFinished();
		}
		else {
			delete action;
		}
		endPoint = startPoint;
		return;
}


