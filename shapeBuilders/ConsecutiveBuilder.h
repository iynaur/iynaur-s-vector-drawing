#pragma once
#include "ShapeBuilderBase.h"

template <typename T>
class  ConsecutiveBuilder :
	public ShapeBuilderBase //, public QObject
{
	//Q_OBJECT
public:
	ConsecutiveBuilder(){}
	virtual ~ConsecutiveBuilder(){}
	virtual void mouseDown(QPointF p, bool bLeft = true) {
		if (m_Shape == nullptr) {
			m_Shape = static_pointer_cast<GeneralShape>(shared_ptr<T>(new T));
			this->publishShapeCreated();
		}
		m_Shape->appendPoint(p);
	}
	virtual void mouseMove(QPointF p, bool bLeft = true) {
		if (m_Shape)m_Shape->appendPoint(p);
	}
	virtual void mouseUp(QPointF p, bool bLeft = true) {
		this->publishShapeFinished();
		m_Shape = nullptr;
	}

//signals:
//	void shapeCreated();
//	void shapeFinished();
};

