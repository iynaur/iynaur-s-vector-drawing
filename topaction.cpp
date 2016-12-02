#include "topaction.h"

TopAction::TopAction()
{

}
ActionType TopAction::actiontype(){
    return Top;
}
QString TopAction::name(){
    return "Top";
}
void TopAction::undo(){
    for(int i=shapes.size()-1;i>=0;i--){
        allShapes->removeOne(shapes.at(i));
        allShapes->insert(indexOfShapes.at(i),shapes.at(i));
    }
    pickedShapes=shapes;
}
void TopAction::redo(){
    for(int i=0;i<shapes.size();i++){
        allShapes->removeOne(shapes.at(i));
        allShapes->append(shapes.at(i));
    }
    pickedShapes=shapes;
}
