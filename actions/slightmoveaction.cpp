#include "slightmoveaction.h"

SlightMoveAction::SlightMoveAction()
{
    _id=1;
    setText(name());
}
 int SlightMoveAction::id() const{
    return 1;
}
bool SlightMoveAction::mergeWith(const QUndoCommand *other){
    if (static_cast<const SlightMoveAction*>(other)->_id==-1) return false;
    dpoint=dpoint+static_cast<const SlightMoveAction*>(other)->dpoint;
    return true;

}
