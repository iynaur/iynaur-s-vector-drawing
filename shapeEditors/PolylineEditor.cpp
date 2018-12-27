#include "PolylineEditor.h"
#include "editaction.h"
#include "KeyPointMoveAction.h"
#include "AddPointAction.h"
#include "DelPointAction.h"
#include "polyline.h"
PolylineEditor::PolylineEditor()
{
}


PolylineEditor::~PolylineEditor()
{
}

shared_ptr<GeneralShape>& PolylineEditor::shape()
{
	return m_shape;
}

AbstractAction*& PolylineEditor::action()
{
	return m_action;
}

double & PolylineEditor::zoomRatio()
{
	return m_zoomRatio;
}

bool PolylineEditor::mouseDown(QPointF p, bool bLeft, QMouseEvent * event)
{
	if (isRotationHandlePoint(p, m_shape, m_zoomRatio)) {//rotate pickedShape

															   //qDebug()<<"rotate pickedShape";
		currentMouseHanded = RotationPoint;
		endPoint = /*startPoint = */p;
		pickedShapestartRotationangle = m_shape->Rotationangle;
		return 1;

	}
	else if (scaleHandlePointId(p, m_shape, m_zoomRatio)!=-1) {//scale pickedShape
															//qDebug()<<"scale pickedShape";
		startScale(p);
		return 1;
	}
	else {
		if (m_shape->minDistance(p)*m_zoomRatio > 4) return 0;
		currentMouseHanded = None;
		auto cp = m_shape->centralPoint();
		Polyline *tmp = new Polyline;
		tmp->points = m_shape->points;
		tmp->drag(QPointF(0,0)-cp);
		tmp->zoom(m_shape->getsx(), m_shape->getsy());
		double sita = -m_shape->Rotationangle / 180 * M_PI;
		for (int i = 0; i < tmp->points.size();i++) {
			QPointF mp = tmp->points[i];
			auto vmp = rotated(mp, sita)+cp;
			if (distance(p, vmp)*m_zoomRatio < 6) {
				currentMouseHanded = KeyPoint;
				m_keyPointIdx = i;
				endPoint = startPoint = vmp;
				return 1;
			}
		}
		
		endPoint = startPoint = p;
		return 1;
	}
}

void PolylineEditor::mouseMove(QPointF p, bool bLeft, QMouseEvent * event)
{
	if (currentMouseHanded == RotationPoint) {//Rotation
											  //qDebug()<<"RotationPoint";
		QPointF centralpoint = QPointF((m_shape->minx + m_shape->maxx) / 2,
			(m_shape->miny + m_shape->maxy) / 2);

		qreal sita = acos(-(p.y() - centralpoint.y()) / sqrt(
			(p.x() - centralpoint.x())*
			(p.x() - centralpoint.x()) +
			(p.y() - centralpoint.y())*
			(p.y() - centralpoint.y())));
		if (p.x() - centralpoint.x() < 0) {
			sita = -sita;
		}
		if (m_shape->getsy() < 0) {
			sita = sita + M_PI;//解决图形倒置时的bug
		}
		m_shape->setRotationangle(sita / M_PI * 180);
		return;
	}
	else if (currentMouseHanded == ScalePoint) {//scale,NEED REWRITE!!!
										   //qDebug()<<"ScalePoint";
		doScale(p);
		return;
	}
	else if (currentMouseHanded == KeyPoint) {
		auto oldcp = m_shape->centralPoint();
		double sita = -m_shape->Rotationangle / 180 * M_PI;
		auto un_rot_p = rotated(p - oldcp, -sita);
		auto un_scale_p = QPointF(un_rot_p.x() / m_shape->getsx(), un_rot_p.y() / m_shape->getsy());

		m_shape->points[m_keyPointIdx] = un_scale_p+ oldcp;
		m_shape->updateRange();
		auto newcp= m_shape->centralPoint();
		auto rp = m_shape->points[m_keyPointIdx]-newcp;
		auto scale_p = QPointF(rp.x() * m_shape->getsx(), rp.y() * m_shape->getsy());
		auto rot_p= rotated(scale_p, sita);
		auto d = p - (rot_p+newcp);
		m_shape->drag(d);
		endPoint = p;
	}
	else {

		m_shape->drag(p - endPoint);
		endPoint = p;
		return;
	}
}

void PolylineEditor::mouseUp(QPointF realPoint, bool bLeft, QMouseEvent * event)
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
		return;
	}
	else if (currentMouseHanded == KeyPoint) {
		KeyPointMoveAction* action = new KeyPointMoveAction();
		m_shape->updateRange();
		action->shapes.append(m_shape);
		action->dpoint = endPoint - startPoint;
		action->m_id = m_keyPointIdx;
		if (action->dpoint != QPointF(0, 0)) {
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

void PolylineEditor::publishEditFinished()
{
	m_callBack->call("editFinished");
}

void PolylineEditor::setCallBack(ICallBack * callBack)
{
	m_callBack = callBack;
}

void PolylineEditor::drawControlPoints(QPainter & painter, qreal zoomRatio)
{
	painter.translate((m_shape->minx + m_shape->maxx) / 2 * zoomRatio, (m_shape->miny + m_shape->maxy) / 2 * zoomRatio);
	painter.rotate(m_shape->Rotationangle);
	painter.setPen(QPen(Qt::black, 3));
	auto cp = m_shape->centralPoint();
	for(auto p: m_shape->points)
		painter.drawPoint( (p-cp).x() * zoomRatio*(m_shape->getsx()), (p - cp).y() * zoomRatio*(m_shape->getsy()));
	painter.rotate(-(m_shape->Rotationangle));
	painter.translate(-((m_shape->minx + m_shape->maxx) / 2)*zoomRatio, -((m_shape->miny + m_shape->maxy) / 2)*zoomRatio);
}

QList<QAction*> PolylineEditor::QActions(QPointF p, QContextMenuEvent * event)
{
	auto res = QList<QAction*>();
	m_contextMenuPoint = p;

	auto actionDelPoint = new QAction(tr("DelPoint"), this);
	actionDelPoint->setIcon(QIcon(QString::fromUtf8("./image/blue-del-sign-md.png")));
	connect(actionDelPoint, &QAction::triggered, this, &PolylineEditor::delPoint);
	actionDelPoint->setEnabled(false);
	res.append(actionDelPoint);
	if (getDelId()) {
		actionDelPoint->setEnabled(true);
	}

	auto actionAddPoint = new QAction(tr("AddPoint"), this);
	actionAddPoint->setIcon(QIcon(QString::fromUtf8("./image/blue-plus-sign-md.png")));
	connect(actionAddPoint, &QAction::triggered, this, &PolylineEditor::addPoint);
	actionAddPoint->setEnabled(false);
	res.append(actionAddPoint);
	if (getAddId()) {
		actionAddPoint->setEnabled(true);
	}

	return res;
}

void PolylineEditor::addPoint()
{
	auto tmp= new AddPointAction();
	tmp->shapes.append(m_shape);
	tmp->m_id = m_addId;
	tmp->newPoint = m_contextMenuPoint;
	m_action = tmp;
	publishEditFinished();
	return;
}

void PolylineEditor::delPoint()
{
	//for (int i = 0; i < m_shape->points.size(); i++) {
	//	if (distance(m_contextMenuPoint, m_shape->points[i]) * m_zoomRatio < 5) {
	auto tmp = new DelPointAction();
	tmp->shapes.append(m_shape);
	tmp->m_id = m_delId;
	auto spPolyline = dynamic_pointer_cast<Polyline>(m_shape);
	tmp->newPoint = spPolyline->getVisualPoint(m_shape->points[m_delId]);
	m_action = tmp;
	publishEditFinished();
	return;
	//	}
	//}
}

bool PolylineEditor::getDelId()
{
	if (m_shape->points.size() > 2) {
		auto cp = m_shape->centralPoint();
		Polyline *tmp = new Polyline;
		tmp->points = m_shape->points;
		tmp->drag(QPointF(0, 0) - cp);
		tmp->zoom(m_shape->getsx(), m_shape->getsy());
		double sita = -m_shape->Rotationangle / 180 * M_PI;
		for (int i = 0; i < tmp->points.size(); i++) {
			QPointF mp = tmp->points[i];
			auto vmp = rotated(mp, sita) + cp;
			if (distance(m_contextMenuPoint, vmp)*m_zoomRatio < 6) {
				m_delId = i;
				return true;
			}
		}
		//
		//for (int i = 0; i < m_shape->points.size(); i++) {
		//	if (distance(m_contextMenuPoint, m_shape->points[i]) * m_zoomRatio < 5) {
		//		m_delId=i;
		//		return true;
		//	}
		//}
	}
	return false;
}

bool PolylineEditor::getAddId()
{
	//
	auto spPolyline = dynamic_pointer_cast<Polyline>(m_shape);
	for (int i = 0; i < m_shape->points.size() - 1; i++) {
		if (CalculatePointToLineDistance
			(
				m_contextMenuPoint,
				spPolyline->getVisualPoint(spPolyline->points[i]),
				spPolyline->getVisualPoint(spPolyline->points[i + 1])
			)
			*m_zoomRatio < 3)
		{
			m_addId = i + 1;
			return true;
		}
	}
	return false;
}
