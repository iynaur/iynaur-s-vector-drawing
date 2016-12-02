#ifndef SETBRUSHACTION_H
#define SETBRUSHACTION_H
#include<abstractaction.h>


class SetBrushAction : public AbstractAction
{
public:
    SetBrushAction();
    QBrush oldbrush,newbrush;
    //void exec();
    void redo();
    void undo();
    ActionType actiontype();
    virtual QString name();
};

#endif // SETBRUSHACTION_H
