#ifndef GENERATEDLG_H
#define GENERATEDLG_H

#include <QDialog>

namespace Ui {
class GenerateDlg;
}

class GenerateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateDlg(QWidget *parent = nullptr);
    QString const GetFileType(void) {return fileType;}
    QString const GetDataRadix(void) {return dataRadix;}
    QString const GetAddressRadix(void) {return addressRadix;}
    ~GenerateDlg();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GenerateDlg *ui;
    QString fileType;
    QString dataRadix;
    QString addressRadix;
};

#endif // GENERATEDLG_H
