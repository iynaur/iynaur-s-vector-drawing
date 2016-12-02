#include "setbrushaction.h"

SetBrushAction::SetBrushAction()
{

}
void SetBrushAction::redo(){
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
ActionType SetBrushAction::actiontype(){
    return SetBrush;
}
QString SetBrushAction::name(){
    return "SetBrush";
}
