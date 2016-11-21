#ifndef BOTTOMACTION_H
#define BOTTOMACTION_H
#include<abstractaction.h>

class BottomAction : public AbstractAction
{
public:
    BottomAction();
    ActionType actiontype();
    QList<int> indexOfShapes;
};

#endif // BOTTOMACTION_H
