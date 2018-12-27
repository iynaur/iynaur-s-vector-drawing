#include "PolygonEditor.h"
#include "polygon.h"


PolygonEditor::PolygonEditor()
{
}


PolygonEditor::~PolygonEditor()
{
}

bool PolygonEditor::getDelId()
{
	if (m_shape->points.size() > 3) {
		auto cp = m_shape->centralPoint();
		Polygon *tmp = new Polygon;
		tmp->points = m_shape->points;
		tmp->drag(QPointF(0, 0) - cp);
		tmp->zoom(m_shape->getsx(), m_shape->getsy());
		double sita = -m_shape->Rotationangle / 180 * M_PI;
		for (int i = 0; i < tmp->points.size(); i++) {
			QPointF mp = tmp->points[i];
			auto vmp = rotated(mp, sita) + cp;
			if (distance(m_contextMenuPoint, vmp)*m_zoomRatio < 6) {
				m_delId = i;
				return true;
			}
		}
	}
	return false;
}

bool PolygonEditor::getAddId()
{
	auto spPolygon = dynamic_pointer_cast<Polygon>(m_shape);
	for (int i = 0; i < m_shape->points.size(); i++) {
		if (CalculatePointToLineDistance
		(
			m_contextMenuPoint,
			spPolygon->getVisualPoint(m_shape->points[i]),
			spPolygon->getVisualPoint(m_shape->points[(i + 1) % m_shape->points.size()])
		)
			*m_zoomRatio < 3)
		{
			m_addId = (i + 1) % m_shape->points.size();
			return true;
		}
	}
	return false;
}
