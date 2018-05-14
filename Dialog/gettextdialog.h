#ifndef GETTEXTDIALOG_H
#define GETTEXTDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtWidgets/QDialogButtonBox>
#include"LineEdit.h"
#include "dialog_global.h"

class DIALOG_EXPORT GetTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetTextDialog(QWidget *parent = 0);
    ~GetTextDialog();
    QString text;
    void setText(QString text);
    void keyPressEvent(QKeyEvent* e);
    void hidebuttonBox();

private slots:
    void onLineEditTextChanged(const QString &arg1);

private:
    CLineEdit* lineEdit;
    QDialogButtonBox *buttonBox;
    QLabel *label;
};

#endif // GETTEXTDIALOG_H
