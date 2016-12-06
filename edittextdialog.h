#ifndef EDITTEXTDIALOG_H
#define EDITTEXTDIALOG_H

#include <QDialog>
#include"tabwidget.h"

namespace Ui {
class EditTextDialog;
}

class EditTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTextDialog(QWidget *parent = 0);
    ~EditTextDialog();
    QFont newfont;
    QString newtext;
    QColor newcolor;
    TabWidget* tw;
    void udate();
public slots:
    //void onTextChanged();
    void onColorChanged(const QColor &color);
    void onFontChanged(const QFont &font);

private:
    Ui::EditTextDialog *ui;


};

#endif // EDITTEXTDIALOG_H
