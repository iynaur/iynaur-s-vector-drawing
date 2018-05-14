#ifndef DELETEACTION_H
#define DELETEACTION_H
#include<abstractaction.h>


class ACTIONS_EXPORT DeleteAction : public AbstractAction
{
public:
    DeleteAction();
    //DeleteAction(GeneralShape* sp,int index);
    QList<int> indexOfShapes;
    virtual QString name();
    void undo();
    void reallyDo();
};

#endif // DELETEACTION_H
