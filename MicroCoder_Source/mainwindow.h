#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTableView>
#include <QProxyStyle>
#include <QStyle>
#include <QString>
#include <QFile>
#include "draggableheaderview.h"
#include "microprograminstruction.h"
#include "programinstructionlist.h"
#include "configuration.h"
#include "programtablemodel.h"
#include "binaryspindelegate.h"
#include "stringcombodelegate.h"
#include "stringspindelegate.h"
#include "stringcombodelegate.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
   void IndicateParseError(ParseError const & s);
   void IndicateNonParseError(QString const & e);

   void OpenAndValidateInFile(QFile & file);
   void OpenAndValidateOutFile(QFile & file, QString const & displayText, QString const & fileType);
   void ReadConfiguration(Tokenizer & t);
   void WriteOut(bool configOnly = false);
   void WriteTdf(QString addressRadix, QString dataRadix);
   void WriteMif (QString addressRadix, QString dataRadix);
   void EraseProgram(void);
   void DestroyTable(void);
   int StrRadixToInt(QString radix);

   void CreateTable();
    ~MainWindow();

private slots:
    void on_actionLoad_Microcode_triggered();

    void on_actionEdit_Configuration_triggered();

    void on_actionSave_MicroCode_triggered();

    void on_actionLoad_Configuration_triggered();

    void on_actionSave_Configuration_triggered();

    void on_actionNew_Microcode_triggered();

    void on_actionNew_Configuration_triggered();

    void on_actionAdd_Instruction_triggered();

    void on_actionExit_triggered();

    void on_actionGenerate_triggered();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void headerContextMenuRequested(const QPoint &pos);

    void on_actionBinary_triggered();

    void on_actionHexadecimal_triggered();

    void on_actionDecimal_triggered();

    void on_actionAdd_Instruction_Context_triggered();

    void on_actionDelete_Instruction_Context_triggered();

private:
    Ui::MainWindow *ui;
    QString activePath;
    Configuration theConfiguration;
    ProgramInstructionList theProgram;
    QTableView * theView;
    ProgramTableModel * theModel;
    TargetComboDelegate * targetDelegate;
    CondSpinDelegate * conditionDelegate;
    QAbstractItemDelegate * dummyDelegate;
    QAbstractItemModel * dummyModel;
    DraggableHeaderView * draggableHeader;
    QList<BinarySpinDelegate *> valueDelegates;
    int contextColumn;
    int contextRow;


};

#endif // MAINWINDOW_H
