#include "gettextdialog.h"
#include "ui_gettextdialog.h"

GetTextDialog::GetTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetTextDialog)
{
            //QString currentText="hello world!";
            //text="hello world!";
    ui->setupUi(this);
    lineEdit=new CLineEdit;
    ui->verticalLayout->insertWidget(1,lineEdit);
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(onLineEditTextChanged(QString)));
    setWindowTitle(tr("Enter Content"));

            lineEdit->setText(text);
            //ui->buttonBox->hide();


}
void GetTextDialog::keyPressEvent(QKeyEvent* e){

}

GetTextDialog::~GetTextDialog()
{
    delete ui;
}
void GetTextDialog::setText(QString text){
    this->text=text;
    lineEdit->setText(this->text);
    lineEdit->setFocus(Qt::OtherFocusReason);
    lineEdit->selectAll();
}

void GetTextDialog::onLineEditTextChanged(const QString &arg1)
{

    text=arg1;
    //qDebug()<<"text"<<text;
}
void GetTextDialog::hidebuttonBox(){
    ui->buttonBox->hide();
}
