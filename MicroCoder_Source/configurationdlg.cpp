#include "configurationdlg.h"
#include "ui_configurationdlg.h"
#include <QMessageBox>
#include <QMenu>
#include <QMimeData>
#include <QItemSelectionModel>
#include "signalconfigdlg.h"

ConfigurationDlg::ConfigurationDlg(Configuration const & initial, QWidget *parent) :
   config(initial),
    QDialog(parent),
    ui(new Ui::ConfigurationDlg)
{
    ui->setupUi(this);
//    theList << "Hello" << "world" <<"string" << "box" <<"I" << "am" << "going" << "to" << "hold" << "the" << "conditions";
    pModel = new MyStringListModel(config.GetConditions(),this);
    ui->tableView->setModel(pModel);
    //ui->signalList->setModel(pModel);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

/*    SignalList sl;
    sl.appendNew(UCSignal(QString("LD_A"),1));
    sl.appendNew(UCSignal(QString("OE_X"),1));
    sl.appendNew(UCSignal(QString("OP"),3));*/
    pSignalModel = new SignalListModel(config.GetSignals(),this);
    ui->signalList->setModel(pSignalModel);
    pBSD = new BinarySpinDelegate(1,(1<<15),10,ui->signalList);
    ui->signalList->setItemDelegateForColumn(1,pBSD);
    ui->signalList->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->signalList->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->signalList->setContextMenuPolicy(Qt::CustomContextMenu);
}

Configuration const & ConfigurationDlg::GetConfiguration()
{
   config.SetConditions(pModel->stringList());
   config.SetSignals(pSignalModel->GetSignals());
   return config;
}

ConfigurationDlg::~ConfigurationDlg()
{
   delete pModel;
   delete pSignalModel;
   delete pBSD;
   delete ui;
}

void ConfigurationDlg::on_listView_customContextMenuRequested(const QPoint &pos)
{
    popupPoint = pos;
    QMenu menu(this);
    menu.addAction(ui->actionDelete);
    menu.addAction(ui->actionAddTarget);
    menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
}

void ConfigurationDlg::on_listView_clicked(const QModelIndex &index)
{

}

void ConfigurationDlg::on_actionDelete_triggered()
{
   QItemSelectionModel * psel = ui->tableView->selectionModel();
   QModelIndexList const & selection = psel->selectedRows();
   if (selection.size() == 0)
   {
      QMessageBox msg;
      msg.setText("Select a row first before attempting deletion.");
      msg.exec();
      return;
   }
   pModel->DeleteSelection(selection);
}

void ConfigurationDlg::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    popupPoint = pos;
    QMenu menu(this);
    menu.addAction(ui->actionDelete);
    menu.addAction(ui->actionAddTarget);
    menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
}
void ConfigurationDlg::on_actionDeleteSignal_triggered()
{
   QItemSelectionModel * psel = ui->signalList->selectionModel();
   QModelIndexList const & selection = psel->selectedRows();
   if (selection.size() == 0)
   {
      QMessageBox msg;
      msg.setText("Select a row first before attempting deletion.");
      msg.exec();
      return;
   }
   pSignalModel->DeleteSelection(selection);
}

void ConfigurationDlg::on_actionAddTarget_triggered()
{
   MyStringListModel * theModel =  static_cast<MyStringListModel*>(ui->tableView->model());
   theModel->AddNew();
   ui->tableView->selectRow(theModel->rowCount()-1);
}

void ConfigurationDlg::on_actionAddSignal_triggered()
{
   SignalConfigDlg dlg;
   int code = dlg.exec();
   if(code == QDialog::Accepted)
   {
      UCSignal newSignal = dlg.NewSignal();
      pSignalModel->AddSignal(newSignal);
   }
}

void ConfigurationDlg::on_signalList_customContextMenuRequested(const QPoint &pos)
{
    popupPoint = pos;
    QMenu menu(this);
    menu.addAction(ui->actionAddSignal);
    menu.addAction(ui->actionDeleteSignal);
    menu.exec(ui->signalList->viewport()->mapToGlobal(pos));
}
