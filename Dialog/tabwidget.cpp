#include "tabwidget.h"
//#include "ui_tabwidget.h"

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)//,
    //ui(new Ui::TabWidget)
{
    //ui->setupUi(this);
    fdlg=new MyFontDialog;
    fdlg->setOption(QFontDialog::NoButtons);
    fdlg->setOption(QFontDialog::DontUseNativeDialog);
    //resize(fdlg->size());
    addTab(fdlg,tr("Font"));


     gtdlg=new GetTextDialog;
     gtdlg->hidebuttonBox();
    addTab(gtdlg,tr("Content"));



}


TabWidget::~TabWidget()
{
    //delete ui;
    delete fdlg;
    delete gtdlg;
}
