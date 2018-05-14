#pragma once
#include <QObject>
#include "ShapeEditorBase.h"
#include "mywidget.h"
#include "RotateWidget.h"
class TextEditor :
	public QObject, public ShapeEditorBase
{
	Q_OBJECT
public:
	TextEditor();
	~TextEditor();
	virtual shared_ptr<GeneralShape>& shape();
	virtual AbstractAction*& action();
	virtual double& zoomRatio();
	virtual bool mouseDown(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseMove(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual void mouseUp(QPointF p, bool bLeft = true, QMouseEvent * event = nullptr);
	virtual bool keyDown(QKeyEvent *event);
	virtual bool keyUp(QKeyEvent *event);
	virtual void publishEditFinished();
	virtual void setCallBack(ICallBack* callBack);
	void virtual drawControlPoints(QPainter &painter, qreal zoomRatio);
	bool contextMenu(QContextMenuEvent * event);
	virtual QList<QAction*> QActions(QPointF p = QPointF(), QContextMenuEvent * event = nullptr);

public slots:
	void currentTextChanged(QString s);
	void editText();
private:
	MyLineEdit* currentLine;
	RotateWidget* currentRotateLine;
	bool m_isEditText;
};

