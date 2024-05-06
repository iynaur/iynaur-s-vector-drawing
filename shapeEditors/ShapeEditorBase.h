#pragma once
#include <QWidget>
#include "IShapeEditor.h"
#include "shapeEditors_global.h"

class ShapeEditorBase :
	public IShapeEditor
{
public:
	ShapeEditorBase();
	virtual ~ShapeEditorBase();
	virtual bool contextMenu(QContextMenuEvent * event);
	virtual QList<shared_ptr<GeneralShape>>& shapes();
	virtual shared_ptr<GeneralShape>& shape();
	virtual AbstractAction*& action();
	virtual void setCallBack(ICallBack* callBack);
	virtual double& zoomRatio();
	void virtual drawControlPoints(QPainter &painter, qreal zoomRatio);
	void publishEditFinished();
	virtual bool keyDown(QKeyEvent *event);
	virtual bool keyUp(QKeyEvent *event) ;
	virtual QList<QAction*> QActions(QPointF p = QPointF(), QContextMenuEvent * event = nullptr);
	void startScale(QPointF p);
	void doScale(QPointF p);
	void finishScale(QPointF p);
	virtual void resetHasPicked();
    virtual void setHasPicked() override
    {
        m_hasPicked = 1;
    }
    void clear() override;

	shared_ptr<GeneralShape> m_shape;
    QList<shared_ptr<GeneralShape>> m_shapes;
	AbstractAction* m_action;
    ICallBack* m_callBack = nullptr;
	double m_zoomRatio;
	//bool m_bKeepCenterWhenScale;
protected:
	double pickedShapestartRotationangle;
	MouseHanded currentMouseHanded;
	QPointF endPoint, startPoint;
	QPointF m_scaleCenter;
	int m_scalePointId;
	int m_hasPicked;

};

