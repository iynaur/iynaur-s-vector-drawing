/*
 * This source file is part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ABSTRACTINSTRUMENT_H
#define ABSTRACTINSTRUMENT_H

#include <QtCore/QObject>
#include <QMouseEvent>
#include <QImage>

//QT_BEGIN_NAMESPACE
//class ImageArea;
//QT_END_NAMESPACE

class DrawAreaWidget;
class AbstractInstrument : public QObject
{
    Q_OBJECT

public:
    explicit AbstractInstrument(QObject *parent = 0);
    virtual ~AbstractInstrument(){}

    virtual void mousePressEvent(QMouseEvent *event, DrawAreaWidget* drawArea) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event, DrawAreaWidget* drawArea) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *event, DrawAreaWidget* drawArea) = 0;
    
signals:
    
protected:
    QPoint mStartPoint, mEndPoint; /**< Point for events. */

    
};

#endif // ABSTRACTINSTRUMENT_H
