#include "codeeditdlg.h"
#include<QGridLayout>
CodeEditDlg::CodeEditDlg(QWidget *parent) :
    QDialog(parent)//,
    //ui(new Ui::CodeEditDlg)
{
    //ui->setupUi(this);
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    buttonBox->setGeometry(QRect(580, 10, 81, 241));
    buttonBox->setOrientation(Qt::Vertical);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    textEdit=new QTextEdit(this);
    QGridLayout* gridLayout = new QGridLayout();
    setLayout(gridLayout);
    gridLayout->addWidget(textEdit,0,0);
    gridLayout->addWidget(buttonBox,0,1);
}

CodeEditDlg::~CodeEditDlg()
{
    //delete ui;
}
