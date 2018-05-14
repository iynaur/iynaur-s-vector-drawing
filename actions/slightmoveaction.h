#ifndef SLIGHTMOVEACTION_H
#define SLIGHTMOVEACTION_H

#include"editaction.h"
class ACTIONS_EXPORT SlightMoveAction : public EditAction
{
public:
    SlightMoveAction();
    int id() const ;
    int _id;
    bool mergeWith(const QUndoCommand *other);
};

#endif // SLIGHTMOVEACTION_H
