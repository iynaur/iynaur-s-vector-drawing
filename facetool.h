#ifndef FACETOOL_H
#define FACETOOL_H

#include"tool.h"
class FaceTool : public Tool
{
public:
    FaceTool();
    void mousePress(QPointF p);
    void mouseMove(QPointF p);
    void mouseRelease(QPointF p);
    void construct();
    shared_ptr<Ellipse> e;
    shared_ptr<Polyline> m1,m2,m;
};

#endif // FACETOOL_H
