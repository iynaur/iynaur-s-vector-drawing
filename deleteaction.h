#ifndef DELETEACTION_H
#define DELETEACTION_H
#include<abstractaction.h>


class DeleteAction : public AbstractAction
{
public:
    DeleteAction();
    //DeleteAction(GeneralShape* sp,int index);
    QList<int> indexOfShapes;
    ActionType actiontype();
    virtual QString name();
};

#endif // DELETEACTION_H
