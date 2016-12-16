#include "setpenaction.h"

SetPenAction::SetPenAction()
{
setText(name());
}
void SetPenAction::redo(){
    foreach(shared_ptr<GeneralShape> sp, shapes){
        sp->setPen(newpen);
    }
    pickedShapes=shapes;
}
void SetPenAction::undo(){
    foreach(shared_ptr<GeneralShape> sp, shapes){
        sp->setPen(oldpen);
    }
    pickedShapes=shapes;
}
ActionType SetPenAction::actiontype(){
    return SetPen;
}
QString SetPenAction::name(){
    return "SetPen";
}
