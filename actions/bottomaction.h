#ifndef BOTTOMACTION_H
#define BOTTOMACTION_H
#include<abstractaction.h>

class ACTIONS_EXPORT BottomAction : public AbstractAction
{
public:
    BottomAction();
    QList<int> indexOfShapes;
    virtual QString name();
    void undo();
    void reallyDo();

};

#endif // BOTTOMACTION_H
