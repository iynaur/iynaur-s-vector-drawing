#pragma once
#include "icallback_global.h"
#include <QWidget>
#include "generalshape.h"
#include "abstractaction.h"
#include "ICallback.h"
/*SHAPEEDITORS_EXPORT*/ enum MouseHanded { None, RotationPoint, ScalePoint, KeyPoint };
class IShapeEditor
{
public:
	
    IShapeEditor() {}
    virtual ~IShapeEditor() {}
	virtual shared_ptr<GeneralShape>& shape() = 0;
	virtual QList<shared_ptr<GeneralShape>>& shapes() = 0;
	virtual QList<QAction*> QActions(QPointF p = QPointF(), QContextMenuEvent * event = nullptr) = 0;
	virtual AbstractAction*& action() = 0;
	virtual double& zoomRatio() = 0;
	virtual bool mouseDown(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr) = 0;
	virtual void mouseMove(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr) = 0;
	virtual void mouseUp(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr) = 0;
	virtual bool keyDown(QKeyEvent *event) = 0;
	virtual bool keyUp(QKeyEvent *event) = 0;
	virtual bool contextMenu(QContextMenuEvent * event)=0;
	virtual void publishEditFinished() = 0;
	virtual void setCallBack(ICallBack* callBack) = 0;
	void virtual drawControlPoints(QPainter &painter, qreal zoomRatio) = 0;
	virtual void resetHasPicked() = 0;

};

ICALLBACK_EXPORT void updateIShapeEditor(const QList<shared_ptr<GeneralShape>> pickedShapes,
    shared_ptr<IShapeEditor>& curIShapeEditor);
