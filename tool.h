#ifndef TOOL_H
#define TOOL_H

#include<QPointF>
//#include"drawareawidget.h"
#include "curve.h"
#include "closecurve.h"
#include "circle.h"
#include "polyline.h"
#include "polygon.h"
#include "rect.h"
#include "ellipse.h"
#include "text.h"
class DrawAreaWidget;
class Tool
{
public:
    Tool();
    virtual void mousePress(QPointF p)=0;
    virtual void mouseMove(QPointF p)=0;
    virtual void mouseRelease(QPointF p)=0;
    DrawAreaWidget* canvas;
};

#endif // TOOL_H
