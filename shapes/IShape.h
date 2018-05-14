#pragma once

#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#include<cmath>
#include<mymath.h>
#include<memory>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

using namespace std;
const double MAX = 10000;
const double  lenthOfRotationHandleLine = 20;

class IShape
{
public:
	IShape();
	virtual ~IShape();

	virtual QString name() = 0;
	virtual shared_ptr<GeneralShape>  copyPaste() = 0;
	QString virtual qStringFromPoints() = 0;
	void virtual fromline(QString line)=0;
	void virtual draw(QPainter &painter, qreal zoomRatio) = 0;
	double virtual minDistance(QPointF point) = 0;
	void  virtual drawClosure(QPainter &painter, qreal zoomRatio)=0;
	QPointF virtual rotationHandlePoint()=0;
	QVector<QPointF> virtual scaleHandlePoints()=0;
	virtual QPointF  centralPoint()=0;
	void virtual zoom(qreal zoomratio)=0;
	void virtual zoom(qreal zx, qreal zy)=0;
	virtual void scale(QPointF from, QPointF to, QPointF center)=0;
	virtual void scale(int pid, QPointF to, QPointF center)=0;

	virtual bool inRange(QPointF p0, QPointF p1)=0;
	void virtual updateRange() = 0;
	virtual void updateBand()=0;
	void virtual drag(QPointF point) = 0;

	bool virtual isEmpty()=0;
	virtual qreal rotationangle()=0;
	virtual void setRotationangle(qreal angle)=0;
	void virtual setPen(const QPen &pen) = 0;
	void virtual setBrush(const QBrush &brush) = 0;
	void virtual setsx(double x)=0;
	void virtual setsy(double y)=0;
	void virtual sets(double y)=0;
	double virtual sy()=0;
	double virtual sx()=0;
	double virtual s()=0;
};

