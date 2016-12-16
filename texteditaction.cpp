#include "texteditaction.h"

TextEditAction::TextEditAction()
{
setText(name());
}
void TextEditAction::undo(){
    dynamic_pointer_cast<Text>(shapes.at(0))->setText(oldtext);
    dynamic_pointer_cast<Text>(shapes.at(0))->setFont(oldfont);
    pickedShapes=shapes;
}
void TextEditAction::redo(){
    dynamic_pointer_cast<Text>(shapes.at(0))->setText(newtext);
    dynamic_pointer_cast<Text>(shapes.at(0))->setFont(newfont);
    pickedShapes=shapes;
}
 ActionType TextEditAction::actiontype(){
     return TextEdit;
 }

 QString TextEditAction::name(){
     return "TextEdit";
 }
