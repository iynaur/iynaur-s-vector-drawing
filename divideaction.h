#ifndef DIVIDEACTION_H
#define DIVIDEACTION_H
#include"abstractaction.h"
#include"combo.h"
class DivideAction : public AbstractAction
{
public:
    DivideAction();
    ~DivideAction();
    Combo* com;
    ActionType actiontype();
    int indexOfCom;
    virtual QString name();

};

#endif // DIVIDEACTION_H
