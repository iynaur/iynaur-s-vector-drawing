#include "abstractshape.h"
 
AbstractShape::AbstractShape()
{
    setBackgroundRole(QPalette::Base);
    minx=miny=maxx=maxy=0;
    Rotationangle=0;
    sx=1;
    sy=1;
}
 
void AbstractShape::setPen(const QPen &pen)
{
    this->pen = pen;
}
 
void AbstractShape::setBrush(const QBrush &brush)
{
    this->brush = brush;
}
