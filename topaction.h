#ifndef TOPACTION_H
#define TOPACTION_H
#include<abstractaction.h>

class TopAction : public AbstractAction
{
public:
    TopAction();
    ActionType actiontype();
    QList<int> indexOfShapes;
};

#endif // TOPACTION_H
