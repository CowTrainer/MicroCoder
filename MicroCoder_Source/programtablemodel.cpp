#include "programtablemodel.h"
#include <algorithm>
#include <QMessageBox>
#include <QAbstractItemView>


ProgramTableModel::ProgramTableModel(Configuration & conf, ProgramInstructionList & prog, QObject * parent)
    : QAbstractTableModel(parent), c(conf),p(prog)
{
}


int ProgramTableModel::rowCount(QModelIndex const & parent) const
{
    if (parent.isValid())
        return 0;
    return p.Size();
}

int ProgramTableModel::columnCount(QModelIndex const & parent) const
{
    if (parent.isValid())
        return 0;
    return 3+c.GetSignals().size();
}

QVariant ProgramTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
       return QVariant();

    if (index.row() >= p.Size())
       return QVariant();


    if ((role != Qt::DisplayRole) && (role != Qt::EditRole) && (role != Qt::BackgroundRole))
        return QVariant();

    MicroProgramInstruction * inst = p.InstructionAt(index.row());
    int col = index.column();

    if(role == Qt::BackgroundRole && col == 1)
    {
        if(!(inst->ValidTarget()))
        {
            QBrush redBackground(Qt::red);
            return redBackground;
        }
        return QVariant();
    }

    if (col == 0)
        return inst->Label();
    else if (col == 1)
        return inst->Target();
    else if (col == 2)
        return inst->Condition();
    else
    {
        int sigLoc = col-3;
        if (sigLoc >= c.GetSignals().size())
            return QVariant();

        UCSignal const & ucs = c.GetSignals().at(sigLoc);
        if (!ucs.InRange(inst->Value(ucs)))
        {
            return "###";
        }
        switch(ucs.RadixInt())
        {
        case 2:
            return QString("%1").arg(inst->Value(ucs), ucs.Width(), ucs.RadixInt(), QChar('0')).toUpper();
        case 16:
            return QString("%1").arg(inst->Value(ucs), ucs.Width()/4, ucs.RadixInt(), QChar('0')).toUpper();
        case 10:
        default:
            return QString::number(inst->Value(ucs),ucs.RadixInt());
        }
    }
}

QVariant ProgramTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant returnValue;
    int sigLoc;

    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            returnValue = QString("Label");
            break;
        case 1:
            returnValue = QString("Target");
            break;
        case 2:
            returnValue = QString("Condition");
            break;
        default:
            sigLoc=section-3;
            returnValue = c.GetSignals().at(sigLoc).Name();
            break;
        }
    }
    else
    {
        returnValue = QString::number(section,10);
    }
    return returnValue;
}

bool ProgramTableModel::setData(const QModelIndex &index, const QVariant &value, int role )
{
    MicroProgramInstruction * inst = p.InstructionAt(index.row());

    int col = index.column();

    if (col==0)
    {
        // I guess we need to make sure labels are valid
        QString newLabel = value.toString();
        if (!(MicroProgramInstruction::ValidLabel(newLabel)))
        {
            QMessageBox msg;
            msg.setText("Label contains a reserved word.");
            msg.exec();
        }
        else if (p.LabelExists(newLabel) && inst->Label() != newLabel)
        {
            QMessageBox msg;
            msg.setText("Label name has already been used");
            msg.exec();
        }
        else
            p.SetLabelAt(index.row(), newLabel);
    }
    else if (col==1)
    {
        QString newTarget = value.toString();
        // We should accept all labels but indicate if invalid for now
        p.SetTargetAt(index.row(), newTarget);
    }
    else if (col==2)
    {
        // Accept whatever we get
        QString newCondition = value.toString();
        inst->SetCondition(newCondition);
    }
    else
    {
        int sigLoc = col-3;
        if (sigLoc >= c.GetSignals().size())
            return false;

        UCSignal const & ucs = c.GetSignals().at(sigLoc);
        p.SetValueAt(ucs,index.row(),value.toInt());
    }

    return false;
}

//bool QAbstractTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
//{

//}

Qt::ItemFlags ProgramTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

void ProgramTableModel::addRow(MicroProgramInstruction * newInstruction)
{
    p.AddInstruction(newInstruction);
}

// Snippet to handle row changes

void ProgramTableModel::rowDropAction(int to, int from)
{
    p.MoveInstruction(from, to);
}

