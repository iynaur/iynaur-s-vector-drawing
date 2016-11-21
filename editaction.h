#ifndef EDITACTION_H
#define EDITACTION_H
#include <QWidget>
#include <QPen>
#include <QBrush>
#include<abstractaction.h>
class EditAction : public AbstractAction
{
public:
    EditAction();
    QPointF dpoint;
    qreal oldRotationangle,newRotationangle;
    qreal dsx,dsy;
    QBrush oldbrush,newbrush;
    ActionType actiontype();

};

#endif // EDITACTION_H
