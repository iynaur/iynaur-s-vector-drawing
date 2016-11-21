#ifndef DRAWACTION_H
#define DRAWACTION_H
#include "GeneralShape.h"
#include <QList>

enum ActionType {Add,Delete,Edit,Combine,Divide,Top,Bottom};

class AbstractAction
{
public:
    AbstractAction();
    QList<GeneralShape *> shapes;
    virtual ActionType actiontype()=0;

};

#endif // DRAWACTION_H
