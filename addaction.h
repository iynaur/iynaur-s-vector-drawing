#ifndef ADDACTION_H
#define ADDACTION_H
#include<abstractaction.h>

class AddAction : public AbstractAction
{
public:
    AddAction();
    AddAction(GeneralShape* sp);
    ActionType actiontype();
    virtual QString name();
};


#endif // ADDACTION_H
