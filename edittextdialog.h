#ifndef EDITTEXTDIALOG_H
#define EDITTEXTDIALOG_H

#include <QDialog>
#include"tabwidget.h"
#include <QtWidgets/QDialogButtonBox>
namespace Ui {
class EditTextDialog;
}

class EditTextDialog : public QDialog
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
