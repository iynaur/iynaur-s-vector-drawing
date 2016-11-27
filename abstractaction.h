#ifndef DRAWACTION_H
#define DRAWACTION_H
#include "generalshape.h"
#include <QList>

enum ActionType {Add,Delete,Edit,Combine,Divide,Top,Bottom};

class AbstractAction
{
public:
    AbstractAction();
    virtual ~AbstractAction();
    QList<GeneralShape *> shapes;
    virtual ActionType actiontype()=0;
    virtual QString name()=0;

};

#endif // DRAWACTION_H
