#ifndef SIGNALCONFIGDLG_H
#define SIGNALCONFIGDLG_H

#include <QDialog>
#include "ucsignal.h"

namespace Ui {
class SignalConfigDlg;
}

class SignalConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SignalConfigDlg(QWidget *parent = 0);
   UCSignal NewSignal(void) const;

    ~SignalConfigDlg();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SignalConfigDlg *ui;
    UCSignal theSignal;
};

#endif // SIGNALCONFIGDLG_H
