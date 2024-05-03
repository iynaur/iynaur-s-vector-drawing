#pragma once
#include "generalshape.h"
//#include <memory>
#include "ICallback.h"
#include "icallback_global.h"
enum Category {
	CurveCategory, CloseCurveCategory, PolylineCategory, PolygonCategory, PickCategory, CircleCategory,
    RectCategory, EllipseCategory, TextCategory, PalmCategory, FaceCategory, ArrowCategory,
    customCategory,
};//Line, Rect, Text,
class IShapeBuilder
{
public:
    IShapeBuilder() {}
    virtual ~IShapeBuilder() {}

	virtual shared_ptr<GeneralShape> shape() = 0;
	virtual void mouseDown(QPointF p, bool bLeft=true) = 0;
	virtual void mouseMove(QPointF p, bool bLeft = true) = 0;
	virtual void mouseUp(QPointF p, bool bLeft = true) = 0;
	virtual void publishShapeCreated()=0;
	virtual void publishShapeFinished()=0;
	virtual void setCallBack(ICallBack* callBack) = 0;
};

typedef vector<pair<QString, Category>> allBuilders;
ICALLBACK_EXPORT shared_ptr<IShapeBuilder> getBuilder(Category c);
ICALLBACK_EXPORT allBuilders getAllBuilders();
