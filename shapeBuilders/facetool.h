#ifndef FACETOOL_H
#define FACETOOL_H

#include<QPointF>
#include"ShapeBuilderBase.h"
#include"shapes.h"
class FaceTool : public ShapeBuilderBase
{
public:
    FaceTool();
	virtual ~FaceTool();
    void mouseDown(QPointF p, bool bLeft = true);
    void mouseMove(QPointF p, bool bLeft = true);
    void mouseUp(QPointF p, bool bLeft = true);
    void construct();
    shared_ptr<Ellipse> e;
    shared_ptr<Polyline> m1,m2,m;
	QPointF p1, p2;
};

#endif // FACETOOL_H
