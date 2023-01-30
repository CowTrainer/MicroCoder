#include "generatedlg.h"
#include "ui_generatedlg.h"

GenerateDlg::GenerateDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateDlg)
{
    ui->setupUi(this);
    QStringList fileTypes = {"MIF", "TDF"};
    QStringList radixes = { "BINARY", "DECIMAL", "HEX" };
    ui->fileBox->insertItems(0,fileTypes);
    ui->dataRadixBox->insertItems(0,radixes);
    ui->addressRadixBox->insertItems(0,radixes);

    // Default value settings of types and radixes
    // Not needed but just incase they are accessed before initalized

    fileType = ui->fileBox->currentText();
    dataRadix = ui->dataRadixBox->currentText();
    addressRadix = ui->addressRadixBox->currentText();
}

GenerateDlg::~GenerateDlg()
{
    delete ui;
}

void GenerateDlg::on_buttonBox_accepted()
{
    fileType = ui->fileBox->currentText();
    dataRadix = ui->dataRadixBox->currentText();
    addressRadix = ui->addressRadixBox->currentText();
}
