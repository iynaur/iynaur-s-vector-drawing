#ifndef DRAWACTION_H
#define DRAWACTION_H
#include "GeneralShape.h"
#include <QList>

enum ActionType {Add,Delete,Edit,Combine,Divide};

class AbstractAction
{
public:
    AbstractAction();
    QList<GeneralShape *> shapes;
    ActionType actiontype;
};

#endif // DRAWACTION_H
