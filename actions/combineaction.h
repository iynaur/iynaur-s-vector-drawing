#ifndef COMBINEACTION_H
#define COMBINEACTION_H
#include<abstractaction.h>
#include<combo.h>
class ACTIONS_EXPORT CombineAction : public AbstractAction
{
public:
    CombineAction();
    ~CombineAction();
    QList<int> indexOfShapes;
    shared_ptr<Combo> com;
    QString name();
    void undo();
    void reallyDo();
};

#endif // COMBINEACTION_H
