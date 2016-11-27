#include "deleteaction.h"

DeleteAction::DeleteAction()
{
    //actiontype=Delete;

}
//DeleteAction::DeleteAction(GeneralShape* sp, int index){
//    actiontype=Delete;
//    shapes.append(sp);
//    indexOfShapes.append(index);
//}
ActionType DeleteAction::actiontype(){
    return Delete;
}
QString DeleteAction::name(){
    return "Delete";
}
