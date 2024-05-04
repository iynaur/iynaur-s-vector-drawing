#include "deleteaction.h"

DeleteAction::DeleteAction()
{
    //actiontype=Delete;
    setText(name());

}
//DeleteAction::DeleteAction(GeneralShape* sp, int index){
//    actiontype=Delete;
//    shapes.append(sp);
//    indexOfShapes.append(index);
//}
QString DeleteAction::name(){
    return "Delete";
}
void DeleteAction::undo(){ // prev crash on qt6?
    //DeleteAction* tmp=static_cast<DeleteAction*>(action);
    for (int i=shapes.size()-1;i>=0;i--){
        allShapes->insert(indexOfShapes.at(i),shapes.at(i));
    }
    pickedShapes=shapes;
}
void DeleteAction::reallyDo(){
    indexOfShapes.clear();
    foreach(shared_ptr<GeneralShape>sp, shapes){
        indexOfShapes.append(allShapes->indexOf(sp));
        allShapes->removeOne(sp);
    }
    pickedShapes.clear();
}
