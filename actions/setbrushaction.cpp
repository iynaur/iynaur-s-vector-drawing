#include "setbrushaction.h"

SetBrushAction::SetBrushAction()
{
setText(name());
}
void SetBrushAction::reallyDo(){
    foreach(shared_ptr<GeneralShape> sp, shapes){
        sp->setBrush(newbrush);
    }
    pickedShapes=shapes;
}
void SetBrushAction::undo(){
    foreach(shared_ptr<GeneralShape> sp, shapes){
        sp->setBrush(oldbrush);
    }
    pickedShapes=shapes;
}
QString SetBrushAction::name(){
    return "SetBrush";
}
