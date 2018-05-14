#include "BaseShapeEditor.h"

#include "editaction.h"
#include<QPainter>

BaseShapeEditor::BaseShapeEditor()
{
}


BaseShapeEditor::~BaseShapeEditor()
{
}

bool BaseShapeEditor::mouseDown(QPointF p, bool bLeft, QMouseEvent * event)
{
	m_hasPicked++;
	if (isRotationHandlePoint(p, m_shape, m_zoomRatio) && m_hasPicked>1) {//rotate pickedShape

														 //qDebug()<<"rotate pickedShape";
		currentMouseHanded = RotationPoint;
		endPoint = startPoint = p;
		pickedShapestartRotationangle = m_shape->Rotationangle;
		return 1;

	}
	else if (scaleHandlePointId(p, m_shape, m_zoomRatio)!=-1 && m_hasPicked>1) {//scale pickedShape
		startScale(p);
		return 1;
	}
	else {
		if (m_shape->minDistance(p)*m_zoomRatio > 4) return 0;
		currentMouseHanded = None;
		
		endPoint = startPoint = p;
		return 1;
	}
}

void BaseShapeEditor::mouseMove(QPointF realPoint, bool bLeft, QMouseEvent * event)
{
	if (currentMouseHanded == RotationPoint) {//Rotation
		QPointF centralpoint = QPointF((m_shape->minx + m_shape->maxx) / 2,
			(m_shape->miny + m_shape->maxy) / 2);

		qreal sita = acos(-(realPoint.y() - centralpoint.y()) / sqrt(
			(realPoint.x() - centralpoint.x())*
			(realPoint.x() - centralpoint.x()) +
			(realPoint.y() - centralpoint.y())*
			(realPoint.y() - centralpoint.y())));
		if (realPoint.x() - centralpoint.x() < 0) {
			sita = -sita;
		}
		if (m_shape->getsy() < 0) {
			sita = sita + M_PI;//解决图形倒置时的bug
		}
		m_shape->setRotationangle(sita / M_PI * 180);
	}
	else if (currentMouseHanded == ScalePoint) {//scale,NEED REWRITE!!!
												//qDebug()<<"ScalePoint";
		//endPoint = realPoint;
		doScale(realPoint);
	}
	else {
		m_shape->drag(realPoint - endPoint);
		endPoint = realPoint;
	}
}

void BaseShapeEditor::mouseUp(QPointF realPoint, bool bLeft, QMouseEvent * event)
{
	if (currentMouseHanded == RotationPoint) {//Rotation
		currentMouseHanded = None;
		//qDebug()<<"RotationPoint";
		QPointF centralpoint = QPointF((m_shape->minx + m_shape->maxx) / 2,
			(m_shape->miny + m_shape->maxy) / 2);

		qreal sita = acos(-(realPoint.y() - centralpoint.y()) / sqrt(
			(realPoint.x() - centralpoint.x())*
			(realPoint.x() - centralpoint.x()) +
			(realPoint.y() - centralpoint.y())*
			(realPoint.y() - centralpoint.y())));
		if (realPoint.x() - centralpoint.x() < 0) {
			sita = -sita;
		}
		if (m_shape->getsy() < 0) {
			sita = sita + M_PI;//解决图形倒置时的bug
		}
		m_shape->setRotationangle(sita / M_PI * 180);

		EditAction* action = new EditAction();
		action->shapes.append(m_shape);
		action->oldRotationangle = pickedShapestartRotationangle;
		action->newRotationangle = m_shape->Rotationangle; 
		AbstractAction* tmp = static_cast<AbstractAction*>(action);
		//tmp->pickedShapes=&pickedShapes;
		tmp->m_bHasDone = true;
		m_action = tmp;
		publishEditFinished();
		//expand();
		//Update();
		return;
	}
	else if (currentMouseHanded == ScalePoint) {//scale,NEED REWRITE!!!
		finishScale(realPoint);

		//expand();
		//Update();
		return;
	}
	else {
		EditAction* action = new EditAction();
		action->shapes.append(m_shape);
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
}

void BaseShapeEditor::drawControlPoints(QPainter & painter, qreal zoomRatio)
{
	painter.setPen(QPen(Qt::black, 3));
	for (auto p : m_shape->scaleHandlePoints())
	{
		painter.drawPoint(p);
	}
}



