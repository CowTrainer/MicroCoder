#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configurationdlg.h"
#include "tokenizer.h"
#include "stringspindelegate.h"
#include "generatedlg.h"
#include "draggableheaderview.h"
#include <tgmath.h>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QSizePolicy>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    theModel(0),
    targetDelegate(0),
    conditionDelegate(0),
    dummyDelegate(0),
    dummyModel(0)
{
    ui->setupUi(this);
    QSettings settings("UWO-ECE", "MicroCoder");
    if (settings.contains("Active Path"))
    {
       activePath = settings.value("Active Path").toString();
    }
}

MainWindow::~MainWindow()
{
    // Close window
    EraseProgram();
    DestroyTable();
    delete ui;
}

void MainWindow::OpenAndValidateInFile(QFile & file)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Configuration"),activePath,
                                                    tr("Microprograms (*.upm);;Configuration Files (*.upc)"));
    if (fileName != "")
    {
       QFileInfo theFile(fileName);
       QSettings settings("UWO-ECE", "MicroCoder");
       activePath = theFile.absoluteDir().absolutePath();
       settings.setValue("Active Path",activePath);

       file.setFileName(fileName);
       if (!file.exists())
       {
          throw QString("Specified file name does not exist");
       }
       if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
       {
           throw QString("Selected file cannot be opened. Check file permissions.");
       }
    }
    // Throw false if blank label
    else
    {
        throw false;
    }
}

void MainWindow::ReadConfiguration(Tokenizer &t)
{
    // Read configuration, save configuration and enable save button
    Configuration newConfiguration(t);
    theConfiguration = newConfiguration;
    ui->actionSave_Configuration->setEnabled(true);
}

void MainWindow::EraseProgram()
{
    theProgram.EraseProgram();
}
void MainWindow::on_actionLoad_Microcode_triggered()
{
    try
    {
        QFile file;
        OpenAndValidateInFile(file);
        QTextStream stream(&file);
        Tokenizer t(stream);
        ReadConfiguration(t);

        // If we got here, the tokenizer should be pointing at ENDCONFIGURATION
        t.NextToken();
        Token token = t.GetToken();
        if ((token.Type()!=Token::STRING) || (token.Value()!="PROGRAM"))
            t.Throw("Expected PROGRAM keyword to begin program");
        t.NextToken();
        EraseProgram();
        for(;;)
        {
            token=t.GetToken();
            if (token.Type()==Token::ENDOFFILE)
                t.Throw("Unexpected end of file while parsing program");

            if ((token.Type()==Token::STRING) && (token.Value()=="ENDPROGRAM"))
                break;

            theProgram.AddInstruction(new MicroProgramInstruction(theConfiguration,t));
        }
        theProgram.CheckTargets();
        ui->actionSave_MicroCode->setEnabled(true);
        CreateTable();
        // New and Load microcode delete the thing
    }
    catch(ParseError const & e)
    {
       IndicateParseError(e);
    }
    catch(QString const & e)
    {
        IndicateNonParseError(e);
    }
    // Don't give error if blank, just exit
    catch(bool e)
    {

    }
}

void MainWindow::on_actionEdit_Configuration_triggered()
{
    ConfigurationDlg dlg(theConfiguration);
    int code = dlg.exec();
    if(code == QDialog::Accepted)
    {
       theConfiguration = dlg.GetConfiguration();
       ui->actionNew_Microcode->setEnabled(true);

       if (theProgram.Size()!=0)
       {
           CreateTable();
       }
    }
}

void MainWindow::on_actionSave_MicroCode_triggered()
{
    WriteOut();
}

void MainWindow::IndicateParseError(ParseError const & e)
{
   QString prompt = QString("Error at line %1: %2").arg(e.lineNo).arg(e.s);
   QMessageBox msg;
   msg.setText(prompt);
   msg.exec();
}

void MainWindow::IndicateNonParseError(QString const & e)
{
   QString prompt = QString("Error: %1").arg(e);
   QMessageBox msg;
   msg.setText(prompt);
   msg.exec();
}

void MainWindow::on_actionLoad_Configuration_triggered()
{
    try
    {
        QFile file;
        OpenAndValidateInFile(file);
        QTextStream stream(&file);
        Tokenizer t(stream);
        ReadConfiguration(t);
        ui->actionEdit_Configuration->trigger();
    }
    catch(ParseError const & e)
    {
        IndicateParseError(e);
    }
    catch(QString const & e)
    {
        IndicateNonParseError(e);
    }
    catch(bool e)
    {

    }
}

void MainWindow::OpenAndValidateOutFile(QFile &outFile, QString const & displayText, QString const & fileType)
{
    // Check filetype
    QString extension;
    if (fileType == ".upc")
    {
        extension = tr("Configuration Files (*.upc)");
    }
    else if (fileType == ".upm")
    {
        extension = tr("Microprograms (*.upm)");
    }
    else if (fileType == ".mif")
    {
        extension = tr("Memory Initialization Files (*.mif)");
    }
    else if (fileType == ".tdf")
    {
        extension = tr("Tiled Data Files (*.tdf)");
    }
    else {
        extension = tr("Configuration Files (*.upc);;Microprograms (*.upm);;Memory Initialization Files (*.mif);;Tiled Data Files (*.tdf)");
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr(qPrintable(displayText)),activePath,
                                                    extension);
    if (fileName != "")
    {
       QFileInfo theFile(fileName);
       QSettings settings("UWO-ECE", "MicroCoder");
       activePath = theFile.absoluteDir().absolutePath();
       settings.setValue("Active Path",activePath);
    }
    else
    {
        // Throw false if blank
        throw false;
    }

    outFile.setFileName(fileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw QString("Selected file cannot be opened");
    }
}

void MainWindow::WriteOut(bool configOnly)
{
    if (theConfiguration.Empty())
    {
       QMessageBox msg;
       msg.setText("It is not possible to save a blank configuration");
       msg.exec();
       return;
    }
    try
    {
        QFile outFile;
        if (configOnly || theProgram.Size()==0)
            OpenAndValidateOutFile(outFile, "Save Configuration", ".upc");
        else
            OpenAndValidateOutFile(outFile, "Save Microcode", ".upm");
        QTextStream outStream(&outFile);
        theConfiguration.Write(outStream);
        if (configOnly || theProgram.Size()==0)
            return;
        outStream << "PROGRAM\n";
        for (int i=0;i<theProgram.Size();i++)
            theProgram.InstructionAt(i)->Write(outStream);
        outStream << "ENDPROGRAM\n";
    }
    catch (ParseError e)
    {
        IndicateParseError(e);
    }
    catch (QString e)
    {
        IndicateNonParseError(e);
    }
    // Don't give error if blank, just exit
    catch(bool e)
    {

    }

}

void MainWindow::on_actionSave_Configuration_triggered()
{
    WriteOut(true);
}

void MainWindow::on_actionNew_Microcode_triggered()
{
    if (theProgram.Size()!=0)
    {
        QMessageBox::StandardButton selection = QMessageBox::warning(this,"Warning","Save current program first?",
                                                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
        if (selection == QMessageBox::Yes)
            on_actionSave_MicroCode_triggered();
        theProgram.EraseProgram();
    }
    ui->actionSave_MicroCode->setEnabled(true);
    CreateTable();
}

void MainWindow::DestroyTable()
{
    theView = ui->tableView;

    delete conditionDelegate;
    delete targetDelegate;

    for (int i=0;i<valueDelegates.size();i++)
    {
        theView->setItemDelegateForColumn(i+3,dummyDelegate);
        delete valueDelegates[i];
    }
    valueDelegates.clear();
    delete theModel;
}

void MainWindow::CreateTable()
{
    theView = ui->tableView;
    theView->setModel(NULL);
    DestroyTable();

    theModel = new ProgramTableModel(theConfiguration,theProgram,this);
    theView->setModel(theModel);

    // Header Settings
    draggableHeader = new DraggableHeaderView(Qt::Vertical,this);
    theView->setVerticalHeader(draggableHeader);
    draggableHeader->show();
    draggableHeader->setDropIndicatorShown(true);
    draggableHeader->setSectionsMovable(true);
    draggableHeader->setSectionsClickable(true);
    draggableHeader->setHighlightSections(true);
    QObject::connect(draggableHeader, &DraggableHeaderView::instructionMove, theModel, &ProgramTableModel::rowDropAction);
    QObject::connect(draggableHeader, &DraggableHeaderView::instructionMove, theView, &QTableView::selectRow);

    // Header Context Menu
    draggableHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(draggableHeader, &DraggableHeaderView::customContextMenuRequested, this, &MainWindow::headerContextMenuRequested);

    targetDelegate = new TargetComboDelegate(theProgram,this);
    theView->setItemDelegateForColumn(1,targetDelegate);

    conditionDelegate = new CondSpinDelegate(theConfiguration,this);
    theView->setItemDelegateForColumn(2,conditionDelegate);

    for (int i=0;i<theConfiguration.GetSignals().size();i++)
    {
        UCSignal ucs = theConfiguration.GetSignals().at(i);
        int min = 0;
        int max = (1 << ucs.Width())-1;
        int radix = ucs.RadixInt();
        valueDelegates.append(new BinarySpinDelegate(min,max,radix,theView));
        theView->setItemDelegateForColumn(i+3,valueDelegates[i]);
    }
}

void MainWindow::on_actionNew_Configuration_triggered()
{
   theConfiguration = Configuration();
   ui->actionSave_Configuration->setEnabled(true);
   ui->actionEdit_Configuration->trigger();
}

void MainWindow::on_actionAdd_Instruction_triggered()
{
    MicroProgramInstruction * inst = new MicroProgramInstruction(theConfiguration);
    theView->setModel(NULL);
    theProgram.AddInstruction(inst);
    theView->setModel(theModel);

}

void MainWindow::on_actionGenerate_triggered()
{
    if (theProgram.ErrorExists())
    {
        QMessageBox msg;
        msg.setText("There are unresolved references in your targets");
        msg.exec();
        return;
    }
    if(theProgram.IsEmpty())
    {
        QMessageBox msg;
        msg.setText("Cannot generate from a blank program");
        msg.exec();
        return;
    }
    GenerateDlg gdlg;
    int code = gdlg.exec();
    if(code == QDialog::Accepted)
    {
        if(gdlg.GetFileType() == "MIF")
            WriteMif(gdlg.GetAddressRadix(), gdlg.GetDataRadix());
        if(gdlg.GetFileType() == "TDF")
            WriteTdf(gdlg.GetAddressRadix(), gdlg.GetDataRadix());
    }
}

void MainWindow::WriteMif(QString addressRadix, QString dataRadix)
{
    try
    {
        QFile outFile;
        OpenAndValidateOutFile(outFile, "Generate MIF", ".mif");
        QTextStream outStream(&outFile);


        // Track each label address (non blank)
        QMap<QString,int> theLabels;
        for (int i = 0; i < theProgram.Size(); i++)
        {
            if(theProgram.InstructionAt(i)->Label() != "")
                theLabels.insert(theProgram.InstructionAt(i)->Label(),i);
        }

        int addressBits = qCeil(log2(theProgram.Size()));
        int conditionBits = qCeil(log2(theConfiguration.GetConditions().size()));
        int signalBits = 0;
        SignalList sList = theConfiguration.GetSignals();
        for (int i=0;i<sList.size();i++)
        {
            signalBits += sList.at(i).Width();
        }
        outStream << "WIDTH\t=\t" << addressBits + conditionBits + signalBits << ";\n";
        outStream << "DEPTH\t=\t" << theProgram.Size() << ";\n\n";

        outStream << "ADDRESS_RADIX\t=\t" << addressRadix.left(3) <<";\n";
        outStream << "DATA_RADIX\t=\t" << dataRadix.left(3) <<";\n\n";
        outStream << "CONTENT\tBEGIN\n";

        // Setting a single MicroInsturction with proper sizing that can be reused
        MicroInstruction format = MicroInstruction(addressBits, conditionBits, signalBits);
        for (int i=0;i<theProgram.Size();i++)
        {
            // Convert index to hex, print middle unimportant stuff, and then print hex
            QString address;
            unsigned int targetValue;
            address.setNum(i,StrRadixToInt(addressRadix));
            outStream << address << '\t' << ':' << '\t';
            if(!(theProgram.InstructionAt(i)->Target() == "NEXT"))
            {
                targetValue = theLabels[theProgram.InstructionAt(i)->Target()];
            }
            else
            {
                targetValue = i+1;
            }
            theProgram.InstructionAt(i)->WriteOutput(outStream, format, targetValue, StrRadixToInt(dataRadix));
            outStream << ";\n";
        }
        outStream << "END";
    }
    catch (ParseError e)
    {
        IndicateParseError(e);
    }
    catch (QString e)
    {
        IndicateNonParseError(e);
    }
    catch (bool e)
    {

    }
}

void MainWindow::WriteTdf(QString addressRadix, QString dataRadix)
{
    // TODO: Add warning for last being fall through
    try
    {
        QFile outFile;
        OpenAndValidateOutFile(outFile, "Generate TDF", ".tdf");
        QTextStream outStream(&outFile);


        // Track each label address (non blank)
        QMap<QString,int> theLabels;
        for (int i = 0; i < theProgram.Size(); i++)
        {
            if(theProgram.InstructionAt(i)->Label() != "")
                theLabels.insert(theProgram.InstructionAt(i)->Label(),i);
        }

        int addressBits = qCeil(log2(theProgram.Size()));
        int conditionBits = qCeil(log2(theConfiguration.GetConditions().size()));
        int signalBits = 0;
        SignalList sList = theConfiguration.GetSignals();
        for (int i=0;i<sList.size();i++)
        {
            signalBits += sList.at(i).Width();
        }
        outStream << "SUBDESIGN MicroStore" << "\n{\n";
        outStream << "\tA[" << addressBits-1 << "..0] : \tINPUT;\n";
        outStream << "\tQ[" << addressBits+conditionBits + signalBits-1 << "..0] : \tOUTPUT;\n}\n";

        outStream << "BEGIN\n\n";
        outStream << "\tTABLE\n";
        outStream << "\t\tA[" << addressBits-1 << "..0] => Q[" << addressBits+conditionBits + signalBits-1 << "..0];\n\n";
        // Setting a single MicroInsturction with proper sizing that can be reused
        MicroInstruction format = MicroInstruction(addressBits, conditionBits, signalBits);
        for (int i=0;i<theProgram.Size();i++)
        {
            // Comments here maybe
            QString address;
            unsigned int targetValue;
            address.setNum(i,StrRadixToInt(addressRadix));
            if(StrRadixToInt(addressRadix) == 2)
                outStream << "\t\tB" << address << "\" => ";
            else if (StrRadixToInt(addressRadix) == 16)
                outStream << "\t\tH\"" << address << "\" => ";
            else
                outStream << "\t\t" << address << " => ";

            if(!(theProgram.InstructionAt(i)->Target() == "NEXT"))
            {
                targetValue = theLabels[theProgram.InstructionAt(i)->Target()];
            }
            else
            {
                targetValue = i+1;
            }

            if(StrRadixToInt(dataRadix) == 2)
            {

                outStream << "B\"";
                theProgram.InstructionAt(i)->WriteOutput(outStream, format, targetValue, 2);
                outStream << "\";";
            }
            else if (StrRadixToInt(dataRadix) == 16)
            {
                outStream << "H\"";
                theProgram.InstructionAt(i)->WriteOutput(outStream, format, targetValue, 16);
                outStream << "\";";
            }
            else
                theProgram.InstructionAt(i)->WriteOutput(outStream, format, targetValue, 10);
            outStream << "\n";
        }
        outStream << "\tEND TABLE;\n";
        outStream << "END;";
    }
    catch (ParseError e)
    {
        IndicateParseError(e);
    }
    catch (QString e)
    {
        IndicateNonParseError(e);
    }
    catch (bool e)
    {

    }
}


void MainWindow::on_actionExit_triggered()
{
    // Close window
    EraseProgram();
    DestroyTable();
    this->close();
}



void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    theView = ui->tableView;
    contextColumn = theView->columnAt(pos.x());
    if (contextColumn >= 3)
    {
        QMenu menu(this);
        menu.addAction(ui->actionBinary);
        menu.addAction(ui->actionHexadecimal);
        menu.addAction(ui->actionDecimal);
        menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
    }

}

void MainWindow::headerContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    contextRow = ui->tableView->verticalHeader()->logicalIndexAt(pos);
    menu.addAction(ui->actionAdd_Instruction_Context);
    menu.addAction(ui->actionDelete_Instruction_Context);
    menu.exec(ui->tableView->verticalHeader()->viewport()->mapToGlobal(pos));

}

int MainWindow::StrRadixToInt(QString radix)
{
    if (radix == "HEX")
    {
        return 16;
    }
    if(radix == "BINARY")
    {
        return 2;
    }
    if (radix == "DECIMAL")
    {
        return 10;
    }
    return 0;
}

void MainWindow::on_actionAdd_Instruction_Context_triggered()
{
    MicroProgramInstruction * inst = new MicroProgramInstruction(theConfiguration);
    inst->SetTarget("NEXT");
    // Refresh View
    theProgram.AddInstructionAt(inst,contextRow);
}

void MainWindow::on_actionDelete_Instruction_Context_triggered()
{
    theProgram.DeleteInstruction(contextRow);
}


void MainWindow::on_actionBinary_triggered()
{
    theConfiguration.GetSignals().at(contextColumn-3).SetRadix(2);
}

void MainWindow::on_actionHexadecimal_triggered()
{
    theConfiguration.GetSignals().at(contextColumn-3).SetRadix(16);
}

void MainWindow::on_actionDecimal_triggered()
{
    theConfiguration.GetSignals().at(contextColumn-3).SetRadix(10);
}

