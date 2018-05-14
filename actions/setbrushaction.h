#ifndef SETBRUSHACTION_H
#define SETBRUSHACTION_H
#include<abstractaction.h>


class ACTIONS_EXPORT SetBrushAction : public AbstractAction
{
public:
    SetBrushAction();
    QBrush oldbrush,newbrush;
    //void exec();
    void reallyDo();
    void undo();
    virtual QString name();
};

#endif // SETBRUSHACTION_H
