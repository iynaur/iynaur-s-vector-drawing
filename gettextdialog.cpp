#include "gettextdialog.h"

GetTextDialog::GetTextDialog(QWidget *parent) :
    QDialog(parent)
{
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    label = new QLabel(this);

    QFont font;
    font.setPointSize(20);
    label->setFont(font);

    label->setText(QApplication::translate("GetTextDialog", "Enter text here!", 0));
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    lineEdit=new CLineEdit;

    QGridLayout* gridLayout = new QGridLayout();
    setLayout(gridLayout);
    gridLayout->addWidget(label,0,0);
    gridLayout->addWidget(lineEdit,1,0);
    gridLayout->addWidget(buttonBox,2,0);

    connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(onLineEditTextChanged(QString)));
    setWindowTitle(tr("Enter Content"));

            lineEdit->setText(text);
            //ui->buttonBox->hide();


}
void GetTextDialog::keyPressEvent(QKeyEvent* e){

}

GetTextDialog::~GetTextDialog()
{
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
    buttonBox->hide();
}
