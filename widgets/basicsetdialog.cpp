#include "basicsetdialog.h"
#include "ui_basicsetdialog.h"

BasicSetDialog::BasicSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicSetDialog)
{
    ui->setupUi(this);
}

BasicSetDialog::~BasicSetDialog()
{
    delete ui;
}

void BasicSetDialog::setMaxWScale(int w)
{
    ui->w_input->setMaximum(w);
}

void BasicSetDialog::setMaxHScale(int h)
{
    ui->h_input->setMaximum(h);
}

void BasicSetDialog::closeEvent(QCloseEvent *)
{
    on_buttonBox_accepted();
}

void BasicSetDialog::on_buttonBox_accepted()
{
    wScale_s = ui->w_input->value();
    hScale_s = ui->h_input->value();
}
