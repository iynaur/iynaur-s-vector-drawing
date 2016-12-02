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
void DivideAction::undo(){
    //DivideAction* tmp=static_cast<DivideAction*>(action);
    for(int i=0;i<shapes.size();i++){
        allShapes->removeOne(shapes.at(i));
        getIntoCombo(shapes.at(i),com);

    }
    //shapes.append(tmp->com);
    allShapes->insert(indexOfCom,com);
//    QList<shared_ptr<GeneralShape>> list;
//    list.append(com);
//    pickedShapes =list;
    pickedShapes.clear();
    pickedShapes.append(com);
}
void DivideAction::redo(){
    for(int i=0;i<shapes.size();i++){
        int index=allShapes->indexOf(static_cast<shared_ptr<GeneralShape>> (com));
         allShapes->insert(index,shapes.at(i));

        getOutOfCombo(shapes.at(i),com);

    }
    allShapes->removeOne(com);
    pickedShapes=shapes;
}
void getOutOfCombo(shared_ptr<GeneralShape> sp,shared_ptr<Combo> tmp){
    //int index=shapes.indexOf(static_cast<shared_ptr<GeneralShape>> (tmp));
    QPointF spcentralPoint=sp->centralPoint();
    sp->drag(-spcentralPoint);
    sp->Rotationangle=sp->Rotationangle+tmp->Rotationangle;
    //sp->drag(tmp->centralPoint());
    sp->drag(rotated(spcentralPoint-tmp->centralPoint(),-tmp->Rotationangle/180*M_PI));//rotate done
    //sp->drag(-tmp->centralPoint());
    sp->zoom(tmp->getsx());
    sp->drag(tmp->centralPoint());//scale done
    //shapes.append(sp);
    //shapes.insert(index,sp);
}

void getIntoCombo(shared_ptr<GeneralShape> sp, shared_ptr<Combo> tmp){
//    QPointF spcentralPoint=sp->centralPoint();
//    sp->drag(-spcentralPoint);
//    sp->Rotationangle=sp->Rotationangle+tmp->Rotationangle;
//    sp->drag(rotated(spcentralPoint-tmp->centralPoint(),-tmp->Rotationangle/180*M_PI));//rotate done
//    sp->zoom(tmp->sx);
//    sp->drag(tmp->centralPoint());//scale done
    //shapes.append(sp);

    QPointF spcentralPoint=sp->centralPoint();
    sp->drag(-spcentralPoint);
    sp->drag(rotated(spcentralPoint-tmp->centralPoint(),tmp->Rotationangle/180*M_PI));
    sp->zoom(1/tmp->getsx());
    sp->Rotationangle=sp->Rotationangle-tmp->Rotationangle;
    sp->drag(tmp->centralPoint());
}
