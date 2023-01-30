#include "signalconfigdlg.h"
#include "ui_signalconfigdlg.h"

SignalConfigDlg::SignalConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignalConfigDlg),
    theSignal("signal",1)
{
    ui->setupUi(this);
    ui->signalNameEdit->setPlainText("signal");
}

UCSignal SignalConfigDlg::NewSignal(void) const
{
   return theSignal;
}


SignalConfigDlg::~SignalConfigDlg()
{
    delete ui;
}

void SignalConfigDlg::on_buttonBox_accepted()
{
   theSignal.ChangeName(ui->signalNameEdit->toPlainText().toUpper());
   theSignal.ChangeWidth(ui->spinBox->value());
}

void SignalConfigDlg::on_buttonBox_rejected()
{

}
