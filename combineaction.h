#ifndef COMBINEACTION_H
#define COMBINEACTION_H
#include<abstractaction.h>
#include<combo.h>
class CombineAction : public AbstractAction
{
public:
    CombineAction();
    ~CombineAction();
    QList<int> indexOfShapes;
    shared_ptr<Combo> com;
    ActionType actiontype();
    QString name();
    void undo();
    void redo();
};

#endif // COMBINEACTION_H
