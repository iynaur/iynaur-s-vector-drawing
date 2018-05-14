#include "addaction.h"
AddAction::AddAction(){
    //actiontype=Add;
    setText(name());

}

AddAction::AddAction(shared_ptr<GeneralShape>sp)
{
    //actiontype=Add;
    shapes.append(sp);

}


QString AddAction::name(){
    return "Add";
}
void AddAction::undo(){
    foreach (shared_ptr<GeneralShape>sp,shapes){
//    del(sp);
    allShapes->removeOne(sp);
    }
    pickedShapes.clear();
}
void AddAction::reallyDo(){
    foreach (shared_ptr<GeneralShape>sp,shapes){
        allShapes->append(sp);
    }
    pickedShapes =  shapes;
}

void AddAction::setPickedShapes()
{
	pickedShapes = shapes;
}
