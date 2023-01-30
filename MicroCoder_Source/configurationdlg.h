#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include "ucsignal.h"
#include "signallistmodel.h"
#include "binaryspindelegate.h"
#include "mystringlistmodel.h"
#include "configuration.h"

namespace Ui {
class ConfigurationDlg;
}

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDlg(Configuration const & initial,QWidget *parent = 0);
    Configuration const & GetConfiguration(void);
    ~ConfigurationDlg();

private slots:
    void on_listView_customContextMenuRequested(const QPoint &pos);

    void on_listView_clicked(const QModelIndex &index);

    void on_actionDelete_triggered();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_actionDeleteSignal_triggered();

    void on_actionAddTarget_triggered();

    void on_actionAddSignal_triggered();

    void on_signalList_customContextMenuRequested(const QPoint &pos);

private:
    Ui::ConfigurationDlg *ui;
    Configuration config;
//    QStringList theList;
    MyStringListModel * pModel;
    SignalListModel * pSignalModel;
    QPoint popupPoint;
    BinarySpinDelegate * pBSD;
};

#endif // CONFIGURATIONDLG_H
