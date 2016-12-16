#ifndef SETPENACTION_H
#define SETPENACTION_H
#include"abstractaction.h"

class SetPenAction : public AbstractAction
{
public:
    SetPenAction();
    QPen oldpen,newpen;
    //void exec();
    void redo();
    void undo();
    ActionType actiontype();
    virtual QString name();
};

#endif // SETPENACTION_H
