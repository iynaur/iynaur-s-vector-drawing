#ifndef TOPACTION_H
#define TOPACTION_H
#include<abstractaction.h>

class TopAction : public AbstractAction
{
public:
    TopAction();
    ActionType actiontype();
    QList<int> indexOfShapes;
    virtual QString name();
    void undo();
    void redo();

};

#endif // TOPACTION_H
