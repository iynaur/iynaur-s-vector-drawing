#include "ruler.h"
#include <QPainterPath>


QDRuler::QDRuler(QDRuler::RulerType rulerType, QWidget* parent)
: QWidget(parent), mRulerType(rulerType), mOrigin(0.), mRulerUnit(1.),
  mRulerZoom(1.), mMouseTracking(false), mDrawText(false)
{
  setMouseTracking(true);
    QFont txtFont("Times", 10,QFont::Light);
    txtFont.setStyleHint(QFont::TypeWriter,QFont::PreferOutline);
    setFont(txtFont);
}

QSize QDRuler::minimumSizeHint() const
{
  return QSize(RULER_BREADTH,RULER_BREADTH);
}

QDRuler::RulerType QDRuler::rulerType() const
{
  return mRulerType;
}

qreal QDRuler::origin() const
{
  return mOrigin;
}

qreal QDRuler::rulerUnit() const
{
  return mRulerUnit;
}

qreal QDRuler::rulerZoom() const
{
  return mRulerZoom;
}



void QDRuler::setOrigin(const qreal origin)
{
  if (mOrigin != origin)
  {
    mOrigin = origin;
    update();
  }
}

void QDRuler::setRulerUnit(const qreal rulerUnit)
{
  if (mRulerUnit != rulerUnit)
  {
    mRulerUnit = rulerUnit;
    update();
  }
}

void QDRuler::setRulerZoom(const qreal rulerZoom)
{
  if (mRulerZoom != rulerZoom)
  {
    mRulerZoom = rulerZoom;
    update();
  }
}


void QDRuler::setCursorPos(const QPoint cursorPos)
{
  mCursorPos = this->mapFromGlobal(cursorPos);
  mCursorPos += QPoint(RULER_BREADTH,RULER_BREADTH);
  update();
}

void QDRuler::setMouseTrack(const bool track)
{
  if (mMouseTracking != track)
  {
    mMouseTracking = track;
    update();
  }
}
void QDRuler::setCursorPos( QMouseEvent* e)
{
  QPoint  cursorPos=e->globalPos();
  mCursorPos = this->mapFromGlobal(cursorPos);
  //mCursorPos += QPoint(RULER_BREADTH,RULER_BREADTH);
  update();
}

void QDRuler::mouseMoveEvent(QMouseEvent* event)
{
  mCursorPos = event->pos();
  update();
  QWidget::mouseMoveEvent(event);
}

void QDRuler::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
    QPen pen(Qt::black,0); // zero width pen is cosmetic pen
    //pen.setCosmetic(true);
    painter.setPen(pen);
  // We want to work with floating point, so we are considering
  // the rect as QRectF
  QRectF rulerRect = this->rect();

  // at first fill the rect
  //painter.fillRect(rulerRect,QColor(220,200,180));
  painter.fillRect(rulerRect,QColor(236,233,216));

  // drawing a scale of 25
  drawAScaleMeter(&painter,rulerRect,25,(Horizontal == mRulerType ? rulerRect.height()
        : rulerRect.width())/2);
  // drawing a scale of 50
  drawAScaleMeter(&painter,rulerRect,50,(Horizontal == mRulerType ? rulerRect.height()
        : rulerRect.width())/4);
  // drawing a scale of 100
  mDrawText = true;
  drawAScaleMeter(&painter,rulerRect,100,0);
  mDrawText = false;

  // drawing the current mouse position indicator
    //painter.setOpacity(0.4);
    painter.setPen(QPen(Qt::green,0));
  drawMousePosTick(&painter);
    painter.setOpacity(1.0);
    painter.setPen(pen);

  // drawing no man's land between the ruler & view
  QPointF starPt = Horizontal == mRulerType ? rulerRect.bottomLeft()
      : rulerRect.topRight();
  QPointF endPt = Horizontal == mRulerType ? rulerRect.bottomRight()
      : rulerRect.bottomRight();
  painter.setPen(QPen(Qt::black,2));
  painter.drawLine(starPt,endPt);
}

void QDRuler::drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin)
{
  // Flagging whether we are horizontal or vertical only to reduce
  // to cheching many times
  bool isHorzRuler = Horizontal == mRulerType;

  scaleMeter  = scaleMeter * mRulerUnit * mRulerZoom;

  // Ruler rectangle starting mark
  qreal rulerStartMark = isHorzRuler ? rulerRect.left() : rulerRect.top();
  // Ruler rectangle ending mark
  qreal rulerEndMark = isHorzRuler ? rulerRect.right() : rulerRect.bottom();

  // Condition A # If origin point is between the start & end mard,
  //we have to draw both from origin to left mark & origin to right mark.
  // Condition B # If origin point is left of the start mark, we have to draw
  // from origin to end mark.
  // Condition C # If origin point is right of the end mark, we have to draw
  // from origin to start mark.
  if (mOrigin >= rulerStartMark && mOrigin <= rulerEndMark)
  {
    drawFromOriginTo(painter, rulerRect, mOrigin, rulerEndMark, 0, scaleMeter, startPositoin);
    drawFromOriginTo(painter, rulerRect, mOrigin, rulerStartMark, 0, -scaleMeter, startPositoin);
  }
  else if (mOrigin < rulerStartMark)
  {
        int tickNo = int((rulerStartMark - mOrigin) / scaleMeter);
        drawFromOriginTo(painter, rulerRect, mOrigin + scaleMeter * tickNo,
            rulerEndMark, tickNo, scaleMeter, startPositoin);
  }
  else if (mOrigin > rulerEndMark)
  {
        int tickNo = int((mOrigin - rulerEndMark) / scaleMeter);
    drawFromOriginTo(painter, rulerRect, mOrigin - scaleMeter * tickNo,
            rulerStartMark, tickNo, -scaleMeter, startPositoin);
  }
}

void QDRuler::drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition)
{
  bool isHorzRuler = Horizontal == mRulerType;

  for (qreal current = startMark;
      (step < 0 ? current >= endMark : current <= endMark); current += step)
  {
    qreal x1 = isHorzRuler ? current : rulerRect.left() + startPosition;
    qreal y1 = isHorzRuler ? rulerRect.top() + startPosition : current;
    qreal x2 = isHorzRuler ? current : rulerRect.right();
    qreal y2 = isHorzRuler ? rulerRect.bottom() : current;
    painter->drawLine(QLineF(x1,y1,x2,y2));
    if (mDrawText)
    {
        QPainterPath txtPath;
        txtPath.addText(x1 + 2,y1 + (isHorzRuler ? 15 : -5),
                        this->font(),
                        QString::number(((step/mRulerZoom * startTickNo)+0.5)>0?
                            int((step/mRulerZoom * startTickNo)+0.5)
                                          :-(int(qAbs(step/mRulerZoom * startTickNo)+0.5))));
        startTickNo++;
        if (!isHorzRuler){
        QTransform mat;
        mat.translate(x1,y1);
        mat.rotate(90);
        mat.translate(-x1,-y1);
         txtPath = mat.map(txtPath);
        }
        painter->drawPath(txtPath);
    }
  }
}

void QDRuler::drawMousePosTick(QPainter* painter)
{
  if (mMouseTracking)
  {
    QPoint starPt = mCursorPos;
    QPoint endPt;
    if (Horizontal == mRulerType)
    {
      starPt.setY(this->rect().top());
      endPt.setX(starPt.x());
      endPt.setY(this->rect().bottom());
    }
    else
    {
      starPt.setX(this->rect().left());
      endPt.setX(this->rect().right());
      endPt.setY(starPt.y());
    }
    painter->drawLine(starPt,endPt);
  }
}
