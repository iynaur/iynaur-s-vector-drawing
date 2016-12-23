#include "hlineedit.h"

HLineEdit::HLineEdit()
{

}
void HLineEdit::focusOutEvent(QFocusEvent *event){
    hide();
}
