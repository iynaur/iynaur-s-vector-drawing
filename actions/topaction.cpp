#include "topaction.h"

TopAction::TopAction()
{
setText(name());
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
void TopAction::reallyDo(){
    foreach(shared_ptr<GeneralShape> sp,shapes){
        //action->shapes.append(sp);
        indexOfShapes.append(allShapes->indexOf(sp));
    allShapes->removeOne(sp);
    allShapes->append(sp);
    }
//    for(int i=0;i<shapes.size();i++){
//        allShapes->removeOne(shapes.at(i));
//        allShapes->append(shapes.at(i));
//    }
    pickedShapes=shapes;
}
