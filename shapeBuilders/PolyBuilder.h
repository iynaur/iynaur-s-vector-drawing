#pragma once
#include "ShapeBuilderBase.h"

template <typename T>
class PolyBuilder :
	public ShapeBuilderBase
{
public:
	PolyBuilder(){}
	virtual ~PolyBuilder() {}
	virtual void mouseDown(QPointF p, bool bLeft = true) {

	}
	virtual void mouseMove(QPointF p, bool bLeft = true) {
		if (m_Shape != NULL
			) {
			m_Shape->mouseMove(p);
		}
	}
	virtual void mouseUp(QPointF p, bool bLeft = true) {
		if (m_Shape == NULL) {
			m_Shape = static_pointer_cast<GeneralShape>(shared_ptr<T>(new T));//new Polygon;
			this->publishShapeCreated();
			m_Shape->appendPoint(p);
		}
		m_Shape->removeLastPoint();
		m_Shape->appendPoint(p);
		m_Shape->appendPoint(p);
		if (!bLeft) {//end one Polyline
			m_Shape->removeLastPoint();

			this->publishShapeFinished();
			m_Shape = nullptr;
		}
	}
};

