#include "gettextdialog.h"
#include "ui_gettextdialog.h"

GetTextDialog::GetTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetTextDialog)
{
            //QString currentText="hello world!";
            //text="hello world!";
    ui->setupUi(this);

            ui->lineEdit->setText(text);


}

GetTextDialog::~GetTextDialog()
{
    delete ui;
}
void GetTextDialog::setText(QString text){
    this->text=text;
    ui->lineEdit->setText(this->text);
    ui->lineEdit->setFocus(Qt::OtherFocusReason);
    ui->lineEdit->selectAll();
}

void GetTextDialog::on_lineEdit_textChanged(const QString &arg1)
{

    text=arg1;
    //qDebug()<<"text"<<text;
}
