#include "setpenaction.h"

SetPenAction::SetPenAction()
{
setText(name());
}
void SetPenAction::reallyDo(){
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
QString SetPenAction::name(){
    return "SetPen";
}
