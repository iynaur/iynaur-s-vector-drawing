#ifndef MYCOLORDIALOG_H
#define MYCOLORDIALOG_H
#include<QColorDialog>

class MyColorDialog : public QColorDialog
{
public:
    MyColorDialog();
    void keyPressEvent(QKeyEvent* e);
};

#endif // MYCOLORDIALOG_H
