#pragma once
#include "ShapeBuilderBase.h"

template <typename T>
class TwoPointsShapeBuilder : public ShapeBuilderBase
{
public:
	TwoPointsShapeBuilder() {
		//setMyShape = &setMyShapeByT<T>;
	}
	 ~TwoPointsShapeBuilder() {}
	 virtual void mouseDown(QPointF p, bool bLeft = true) {
		 if (m_Shape == nullptr) {
			 m_Shape = static_pointer_cast<GeneralShape>(shared_ptr<T>(new T));
			 m_Shape->mousePress(p);
			 this->publishShapeCreated();
		 }
	 }
	 virtual void mouseMove(QPointF p, bool bLeft = true) {
		 if (m_Shape != NULL //&& isLeftMouseButtonPressed
			 ) {
			 m_Shape->mouseMove(p);
		 }
	 }
	 virtual void mouseUp(QPointF p, bool bLeft = true) {
		 if (m_Shape == 0) return;
		 m_Shape->removeLastPoint();
		 m_Shape->appendPoint(p);

		 this->publishShapeFinished();
		 m_Shape = nullptr;
	 }

};