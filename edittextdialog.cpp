#include "edittextdialog.h"
#include "ui_edittextdialog.h"

EditTextDialog::EditTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTextDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("EditText"));
//    newfont=;
//    newtext=
    tw=new TabWidget;
    //tw->gtdlg->ui->buttonBox->hide();
            //ui->buttonBox->hide();
    ui->gridLayout->setGeometry(QRect(0,0,tw->size().width(),tw->size().height()));
    ui->gridLayout->addWidget(tw);

    udate();
    connect(tw->fdlg,SIGNAL(currentFontChanged(const QFont &)),
            this,SLOT(onFontChanged(const QFont &))
                );
//    connect(tw->cdlg,SIGNAL(currentColorChanged(const QColor &)),
//            this,SLOT(onColorChanged(const QFont &))
//                );



}
void EditTextDialog::udate(){
    tw->gtdlg->setText(newtext);
    tw->fdlg->setCurrentFont(newfont);
}

EditTextDialog::~EditTextDialog()
{
    delete tw;
    delete ui;
}
void EditTextDialog::onColorChanged(const QColor &color){
    newcolor=color;
}

void EditTextDialog::onFontChanged(const QFont &font){
    newfont=font;
}
