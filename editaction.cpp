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
