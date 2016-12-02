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
void DeleteAction::undo(){
    //DeleteAction* tmp=static_cast<DeleteAction*>(action);
    for (int i=shapes.size()-1;i>=0;i--){
        allShapes->insert(indexOfShapes.at(i),shapes.at(i));
    }
    pickedShapes=shapes;
}
void DeleteAction::redo(){
    foreach(shared_ptr<GeneralShape>sp, shapes){
        allShapes->removeOne(sp);
    }
    pickedShapes.clear();
}
