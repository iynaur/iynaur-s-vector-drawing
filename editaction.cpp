#include "editaction.h"

EditAction::EditAction()
{
    //actiontype=Edit;
    dpoint=QPointF(0,0);
    oldRotationangle=0;
    newRotationangle=0;
    dsx=1;
    dsy=1;
}
ActionType EditAction::actiontype(){
    return Edit;
}
QString EditAction::name(){
    return "Edit";
}
void EditAction::undo(){
    //EditAction* tmp=static_cast<EditAction*>(action);
    foreach (shared_ptr<GeneralShape>sp,shapes){
    sp->drag(-dpoint);
    if (newRotationangle!=oldRotationangle){
        sp->setRotationangle(oldRotationangle);
    }
    sp->setsx(shapes.at(0)->getsx()/dsx);
    sp->setsy(shapes.at(0)->getsy()/dsy);
    }
    pickedShapes=shapes;
}
void EditAction::redo(){
    foreach (shared_ptr<GeneralShape>sp,shapes){
        sp->drag(dpoint);
        if (newRotationangle!=oldRotationangle){
            sp->setRotationangle(newRotationangle);
        }
        sp->setsx(shapes.at(0)->getsx()*dsx);
        sp->setsy(shapes.at(0)->getsy()*dsy);
    }
    pickedShapes=shapes;
}
