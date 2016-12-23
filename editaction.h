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
    qreal oldsx,oldsy,newsx,newsy;
    //QBrush oldbrush,newbrush;
    ActionType actiontype();
    virtual QString name();
    void redo();
    void undo();


};

#endif // EDITACTION_H
