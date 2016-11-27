#include "addaction.h"
AddAction::AddAction(){
    //actiontype=Add;

}

AddAction::AddAction(GeneralShape *sp)
{
    //actiontype=Add;
    shapes.append(sp);

}

ActionType AddAction::actiontype(){
    return Add;
}
QString AddAction::name(){
    return "Add";
}
