#ifndef SETPENACTION_H
#define SETPENACTION_H
#include"abstractaction.h"

class ACTIONS_EXPORT SetPenAction : public AbstractAction
{
public:
    SetPenAction();
    QPen oldpen,newpen;
    //void exec();
    void reallyDo();
    void undo();
    virtual QString name();
};

#endif // SETPENACTION_H
