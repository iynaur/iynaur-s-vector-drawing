#ifndef GETTEXTDIALOG_H
#define GETTEXTDIALOG_H

//#include <QDialog>
//#include <QDebug>
#include <QtWidgets/QDialogButtonBox>
#include"LineEdit.h"

namespace Ui {
class GetTextDialog;
}

class GetTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetTextDialog(QWidget *parent = 0);
    ~GetTextDialog();
    QString text;
    void setText(QString text);
    void keyPressEvent(QKeyEvent* e);
    Ui::GetTextDialog *ui;
    void hidebuttonBox();

private slots:
    void onLineEditTextChanged(const QString &arg1);

private:
    CLineEdit* lineEdit;
    QDialogButtonBox *buttonBox;
    QLabel *label;
};

#endif // GETTEXTDIALOG_H
