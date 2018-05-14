#include "editaction.h"

EditAction::EditAction()
{
    //actiontype=Edit;
    dpoint=QPointF(0,0);
    oldRotationangle=0;
    newRotationangle=0;
    oldsx=newsx=1;
    oldsy=newsy=1;
    setText(name());
}
QString EditAction::name(){
	if (dpoint != QPointF(0, 0))    return "Move";
	else if (oldRotationangle != newRotationangle) return "Rotate";
	else return "Scale";
}
void EditAction::undo(){
    //EditAction* tmp=static_cast<EditAction*>(action);
    foreach (shared_ptr<GeneralShape>sp,shapes){
    sp->drag(-dpoint);
    if (newRotationangle!=oldRotationangle){
        sp->setRotationangle(oldRotationangle);
    }
    if (newsx!=oldsx){
    sp->setsx(oldsx);}
    if (newsy!=oldsy){
    sp->setsy(oldsy);}
    }
    pickedShapes=shapes;
}
void EditAction::reallyDo(){
    foreach (shared_ptr<GeneralShape>sp,shapes){
        sp->drag(dpoint);
        if (newRotationangle!=oldRotationangle){
            sp->setRotationangle(newRotationangle);
        }
        if (newsx!=oldsx){
        sp->setsx(newsx);}
        if (newsy!=oldsy){
        sp->setsy(newsy);}

    }
    pickedShapes=shapes;
}

void EditAction::setPickedShapes()
{
	pickedShapes = shapes;
}
