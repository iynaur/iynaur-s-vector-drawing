#ifndef ADDACTION_H
#define ADDACTION_H
#include"abstractaction.h"

class ACTIONS_EXPORT AddAction : public AbstractAction
{
public:
    AddAction();
    AddAction(shared_ptr<GeneralShape> sp);
    virtual QString name();
    void undo();
    void reallyDo();
	virtual void setPickedShapes();
};


#endif // ADDACTION_H
