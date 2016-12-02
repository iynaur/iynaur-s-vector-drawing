#ifndef ADDACTION_H
#define ADDACTION_H
#include"abstractaction.h"

class AddAction : public AbstractAction
{
public:
    AddAction();
    AddAction(shared_ptr<GeneralShape> sp);
    ActionType actiontype();
    virtual QString name();
    void undo();
    void redo();
};


#endif // ADDACTION_H
