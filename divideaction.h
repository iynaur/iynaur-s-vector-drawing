#ifndef DIVIDEACTION_H
#define DIVIDEACTION_H
#include"abstractaction.h"
#include"combo.h"
class DivideAction : public AbstractAction
{
public:
    DivideAction();
    ~DivideAction();
    shared_ptr<Combo> com;
    ActionType actiontype();
    int indexOfCom;
    virtual QString name();
    void undo();
    void redo();

};
void getOutOfCombo(shared_ptr<GeneralShape> sp,shared_ptr<Combo> tmp);
void getIntoCombo(shared_ptr<GeneralShape> sp, shared_ptr<Combo> tmp);
#endif // DIVIDEACTION_H
