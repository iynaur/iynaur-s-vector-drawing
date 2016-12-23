#ifndef DRAWACTION_H
#define DRAWACTION_H
#include "generalshape.h"

#include <QList>
#include <memory>
#include <QUndoCommand>
#include <QString>
#include <string>
#include <QTranslator>
enum ActionType {Add,Delete,Edit,Combine,Divide,Top,Bottom,SetBrush,TextEdit,SetPen};

class AbstractAction : public QUndoCommand//, public DrawAreaWidget
{
public:
    AbstractAction();
    virtual ~AbstractAction();
    QList<shared_ptr<GeneralShape> > shapes;
    virtual ActionType actiontype()=0;
    virtual QString name()=0;
     QList<shared_ptr<GeneralShape> >* allShapes;
     QList<shared_ptr<GeneralShape> > pickedShapes;

     //friend class DrawAreaWidget;//声明友元类C

};

#endif // DRAWACTION_H
