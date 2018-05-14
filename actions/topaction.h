#ifndef TOPACTION_H
#define TOPACTION_H
#include<abstractaction.h>

class ACTIONS_EXPORT TopAction : public AbstractAction
{
public:
    TopAction();
    QList<int> indexOfShapes;
    virtual QString name();
    void undo();
    void reallyDo();

};

#endif // TOPACTION_H
