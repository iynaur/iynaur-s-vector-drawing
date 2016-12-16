#ifndef RULER_H
#define RULER_H
#define RULER_BREADTH 20
 #include<QWidget>
 #include<QPainter>
 #include<QMouseEvent>
class QDRuler : public QWidget
{
Q_OBJECT
Q_ENUMS(RulerType)
Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)
public:
  enum RulerType { Horizontal, Vertical };
QDRuler(QDRuler::RulerType rulerType, QWidget* parent)
;

QSize minimumSizeHint() const
;
QDRuler::RulerType rulerType() const
;
qreal origin() const
;

qreal rulerUnit() const
;

qreal rulerZoom() const
;

public slots:

void setOrigin(const qreal origin)
;

void setRulerUnit(const qreal rulerUnit)
;

void setRulerZoom(const qreal rulerZoom)
;


void setCursorPos(const QPoint cursorPos)
;

void setMouseTrack(const bool track)
;
public slots:
void setCursorPos( QMouseEvent* e)
;

protected:
void mouseMoveEvent(QMouseEvent* event)
;

void paintEvent(QPaintEvent* event)
;

private:
void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin)
;

void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition)
;

void drawMousePosTick(QPainter* painter)
;
private:
  RulerType mRulerType;
  qreal mOrigin;
  qreal mRulerUnit;
  qreal mRulerZoom;
  QPoint mCursorPos;
  bool mMouseTracking;
  bool mDrawText;
};
#endif // RULER_H
