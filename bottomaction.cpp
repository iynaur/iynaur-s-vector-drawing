#include "bottomaction.h"

BottomAction::BottomAction()
{

}
ActionType BottomAction::actiontype(){
    return Bottom;
}
QString BottomAction::name(){
    return "Bottom";
}
void BottomAction::undo(){
    for(int i=shapes.size()-1;i>=0;i--){
        allShapes->removeOne(shapes.at(i));
        allShapes->insert(indexOfShapes.at(i),shapes.at(i));
    }
    pickedShapes=shapes;
}
void BottomAction::redo(){
    for(int i=0;i<shapes.size();i++){
        allShapes->removeOne(shapes.at(i));
        allShapes->prepend(shapes.at(i));
    }
    pickedShapes =  shapes;
}
