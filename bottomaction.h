#ifndef BOTTOMACTION_H
#define BOTTOMACTION_H
#include<abstractaction.h>

class BottomAction : public AbstractAction
{
public:
    BottomAction();
    ActionType actiontype();
    QList<int> indexOfShapes;
    virtual QString name();
    void undo();
    void redo();

};

#endif // BOTTOMACTION_H
