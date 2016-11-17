#ifndef GETTEXTDIALOG_H
#define GETTEXTDIALOG_H

#include <QDialog>
#include <QDebug>

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

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::GetTextDialog *ui;
};

#endif // GETTEXTDIALOG_H
