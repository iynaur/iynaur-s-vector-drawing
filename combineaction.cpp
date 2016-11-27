#include "combineaction.h"

CombineAction::CombineAction()
{
    //actiontype=Combine;
}
ActionType CombineAction::actiontype(){
    return Combine;
}
QString CombineAction::name(){
    return "Combine";
}
CombineAction::~CombineAction(){
    //delete com;
}
