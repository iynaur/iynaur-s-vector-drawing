#ifndef TEXTEDITACTION_H
#define TEXTEDITACTION_H
#include"abstractaction.h"
#include"text.h"
class ACTIONS_EXPORT TextEditAction : public AbstractAction
{
public:
    TextEditAction();
    QString oldtext,newtext;
    QFont oldfont,newfont;
    void undo();
    void reallyDo();
    virtual QString name();
};

#endif // TEXTEDITACTION_H
