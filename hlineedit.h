#ifndef HLINEEDIT_H
#define HLINEEDIT_H
#include<QLineEdit>

class HLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    HLineEdit();
    void focusOutEvent(QFocusEvent *event);
};

#endif // HLINEEDIT_H
