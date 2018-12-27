#include "ShapeBuilderBase.h"
#include "generalshape.h"
#include"facetool.h"
#include "ShapeBuilderBase.h"
#include "CallBack.h"
#include "PolyBuilder.h"
#include "TwoPointsShapeBuilder.h"
#include "TextBuilder.h"
#include "ConsecutiveBuilder.h"


ShapeBuilderBase::ShapeBuilderBase()
{
}


ShapeBuilderBase::~ShapeBuilderBase()
{
}
void ShapeBuilderBase::publishShapeCreated()
{
	m_callBack->call("shapeCreated");
	//emit shapeCreated();
}

void ShapeBuilderBase::publishShapeFinished()
{
	m_callBack->call("shapeFinished");
	//emit shapeFinished();
}

shared_ptr<GeneralShape> ShapeBuilderBase::shape()
{
	return m_Shape;
}

void ShapeBuilderBase::setCallBack(ICallBack * callBack)
{
	m_callBack = callBack;
}


SHAPEBUILDERS_EXPORT shared_ptr<IShapeBuilder> getBuilder(Category c)
{
	switch (c) {
	case FaceCategory:
	{
		return static_pointer_cast<IShapeBuilder>(shared_ptr<FaceTool>(new FaceTool));
	}
	case CurveCategory: {
		return static_pointer_cast<IShapeBuilder>(shared_ptr<ConsecutiveBuilder<Curve>>(new ConsecutiveBuilder<Curve>));
	}
	case CloseCurveCategory:
	{
		return static_pointer_cast<IShapeBuilder>(shared_ptr<ConsecutiveBuilder<CloseCurve>>(new ConsecutiveBuilder<CloseCurve>));
	}
	case CircleCategory: {
		return static_pointer_cast<IShapeBuilder>(shared_ptr<TwoPointsShapeBuilder<Circle>>(new TwoPointsShapeBuilder<Circle>));
	}
	case RectCategory: {
		return static_pointer_cast<IShapeBuilder>(shared_ptr<TwoPointsShapeBuilder<Rect>>(new TwoPointsShapeBuilder<Rect>));
	}
	case EllipseCategory: {
		return static_pointer_cast<IShapeBuilder>(shared_ptr<TwoPointsShapeBuilder<Ellipse>>(new TwoPointsShapeBuilder<Ellipse>));
	}
	case TextCategory: {
		return static_pointer_cast<IShapeBuilder>(shared_ptr<TextBuilder>(new TextBuilder));
	}
	case PolylineCategory:
	{
		return static_pointer_cast<IShapeBuilder>(shared_ptr<PolyBuilder<Polyline>>(new PolyBuilder<Polyline>));
	}
	case PolygonCategory:
	{
		return static_pointer_cast<IShapeBuilder>(shared_ptr<PolyBuilder<Polygon>>(new PolyBuilder<Polygon>));
	}
	default: return 0;
	}
}

SHAPEBUILDERS_EXPORT allBuilders getAllBuilders() {
	allBuilders all;
	all.push_back({ "curve", CurveCategory });
	all.push_back({ "closecurve", CloseCurveCategory });
	all.push_back({ "polyline", PolylineCategory });
	all.push_back({ "polygon", PolygonCategory });
	all.push_back({ "rect", RectCategory });
	all.push_back({ "ellipse", EllipseCategory });
	all.push_back({ "circle", CircleCategory });
    all.push_back({ "text", TextCategory });
	all.push_back({ "face", FaceCategory });
	return all;
}
