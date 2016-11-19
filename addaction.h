#ifndef ADDACTION_H
#define ADDACTION_H
#include<abstractaction.h>

class AddAction : public AbstractAction
{
public:
    AddAction();
    AddAction(GeneralShape* sp);
};

#endif // ADDACTION_H
