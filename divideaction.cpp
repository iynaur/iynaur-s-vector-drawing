#include "divideaction.h"

DivideAction::DivideAction()
{
    //actiontype=Divide;
}

ActionType DivideAction::actiontype(){
    return Divide;
}
QString DivideAction::name(){
    return "Divide";
}
DivideAction::~DivideAction(){
    //delete com;
}
