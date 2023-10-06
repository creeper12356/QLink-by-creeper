#include "basicsetdialog.h"
#include "ui_basicsetdialog.h"

BasicSetDialog::BasicSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicSetDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&BasicSetDialog::acceptSlot);
}

BasicSetDialog::~BasicSetDialog()
{
    delete ui;
}

void BasicSetDialog::acceptSlot()
{
    qDebug() << "accepted";
    emit basicSetted(QPoint(ui->w_input->value(),ui->h_input->value()));
}

