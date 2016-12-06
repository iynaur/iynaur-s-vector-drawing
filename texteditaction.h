#ifndef TEXTEDITACTION_H
#define TEXTEDITACTION_H
#include"abstractaction.h"
#include"text.h"
class TextEditAction : public AbstractAction
{
public:
    TextEditAction();
    QString oldtext,newtext;
    QFont oldfont,newfont;
    void undo();
    void redo();
    virtual ActionType actiontype();
    virtual QString name();
};

#endif // TEXTEDITACTION_H
