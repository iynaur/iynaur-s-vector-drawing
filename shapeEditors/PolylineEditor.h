#pragma once
#include<QPainter>
#include <QObject>
#include <QAction>
#include "ShapeEditorBase.h"
class PolylineEditor :
	public QObject , public ShapeEditorBase
{
	Q_OBJECT
public:
	PolylineEditor();
	virtual ~PolylineEditor();
	virtual shared_ptr<GeneralShape>& shape();
	virtual AbstractAction*& action();
	virtual double& zoomRatio();
	virtual bool mouseDown(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseMove(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseUp(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
    virtual bool keyDown(QKeyEvent *event) { return 0; }
    virtual bool keyUp(QKeyEvent *event) { return 0; }
	virtual void publishEditFinished();
	virtual void setCallBack(ICallBack* callBack);
	void virtual drawControlPoints(QPainter &painter, qreal zoomRatio);
	virtual QList<QAction*> QActions(QPointF p = QPointF(), QContextMenuEvent * event = nullptr);
	virtual void addPoint();
	virtual void delPoint();
	virtual bool getDelId();
	virtual bool getAddId();
protected:
	int m_keyPointIdx;
	QPointF m_contextMenuPoint;
	int m_delId;
	int m_addId;
	//shared_ptr<Polyline> m_polyline;
};

