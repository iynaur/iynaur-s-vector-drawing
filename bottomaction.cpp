#include "bottomaction.h"

BottomAction::BottomAction()
{
setText(name());
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
    pickedShapes.clear();
    for(int i=0;i<shapes.size();i++){
        indexOfShapes.append(allShapes->indexOf(shapes.at(i)));
        allShapes->removeOne(shapes.at(i));
        allShapes->prepend(shapes.at(i));
        pickedShapes.prepend(shapes.at(i));
    }
    //pickedShapes =  shapes;//order reverse!!!!
}
