#include "combineaction.h"

CombineAction::CombineAction()
{
    //actiontype=Combine;
    setText(name());
}
QString CombineAction::name(){
    return "Combine";
}
CombineAction::~CombineAction(){
    //delete com;
}
void CombineAction::undo(){
    for(int i=shapes.size()-1;i>=0;i--){
        allShapes->insert(indexOfShapes.at(i),shapes.at(i));
    }
    //qDebug()<<shapes.indexOf(tmp->com)<<shapes.size();
    allShapes->removeOne(com);
    pickedShapes =  shapes;
}
void CombineAction::reallyDo(){
    foreach(shared_ptr<GeneralShape> sp,shapes){
        indexOfShapes.append(allShapes->indexOf(sp));
        allShapes->removeOne(sp);
    }

    allShapes->append(com);
    QList<shared_ptr<GeneralShape>> list;
    list.append(com);
    pickedShapes =list;
}
