#include "edittextdialog.h"
#include<QGridLayout>
EditTextDialog::EditTextDialog(QWidget *parent) :
    QDialog(parent)//,
    //ui(new Ui::EditTextDialog)
{
    //ui->setupUi(this);
    buttonBox = new QDialogButtonBox(this);

    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    setWindowTitle(tr("EditText"));

    tw=new TabWidget;

    QGridLayout* gridLayout = new QGridLayout();
    setLayout(gridLayout);
    gridLayout->addWidget(tw,0,0);
    gridLayout->addWidget(buttonBox,1,0);
    resize(450,400);




}


EditTextDialog::~EditTextDialog()
{
    delete tw;
}
