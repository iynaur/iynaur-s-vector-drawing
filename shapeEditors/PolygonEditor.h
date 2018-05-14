#pragma once
#include "PolylineEditor.h"
class PolygonEditor :
	public PolylineEditor
{
	Q_OBJECT
public:
	PolygonEditor();
	virtual ~PolygonEditor();
	virtual bool getDelId();
	virtual bool getAddId();
};

