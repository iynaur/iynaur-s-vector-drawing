#ifndef DIVIDEACTION_H
#define DIVIDEACTION_H
#include"abstractaction.h"
#include"combo.h"
class ACTIONS_EXPORT DivideAction : public AbstractAction
{
public:
    DivideAction();
    ~DivideAction();
    shared_ptr<Combo> com;
    int indexOfCom;
    virtual QString name();
    void undo();
    void reallyDo();

};
void ACTIONS_EXPORT getOutOfCombo(shared_ptr<GeneralShape> sp,shared_ptr<Combo> tmp);
void ACTIONS_EXPORT getIntoCombo(shared_ptr<GeneralShape> sp, shared_ptr<Combo> tmp);
#endif // DIVIDEACTION_H
