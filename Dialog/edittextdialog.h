#ifndef EDITTEXTDIALOG_H
#define EDITTEXTDIALOG_H

#include <QDialog>
#include"tabwidget.h"
#include <QtWidgets/QDialogButtonBox>

class DIALOG_EXPORT EditTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTextDialog(QWidget *parent = 0);
    ~EditTextDialog();
    //QFont newfont;
    //QString newtext;
    //QColor newcolor;
    TabWidget* tw;
    void udate();


private:
QDialogButtonBox *buttonBox;

};

#endif // EDITTEXTDIALOG_H
