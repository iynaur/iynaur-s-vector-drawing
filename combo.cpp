#include "combo.h"

Combo::Combo()
{
    name="Combo";
    minx=miny=maxx=maxy=0;
    Rotationangle=0;
    sx=1;
    sy=1;
    QList<GeneralShape*> list;
    shapes=list;
    QVector<QPointF> vector;
    points=vector;
}

