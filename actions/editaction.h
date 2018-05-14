#ifndef EDITACTION_H
#define EDITACTION_H
#include <QWidget>
#include <QPen>
#include <QBrush>
#include<abstractaction.h>
class ACTIONS_EXPORT EditAction : public AbstractAction
{
public:
    EditAction();
    QPointF dpoint;
    qreal oldRotationangle,newRotationangle;
    qreal oldsx,oldsy,newsx,newsy;
    //QBrush oldbrush,newbrush;
    virtual QString name();
    void reallyDo();
    void undo();
	virtual void setPickedShapes();

};

#endif // EDITACTION_H
