#ifndef PROGRAMTABLEMODEL_H
#define PROGRAMTABLEMODEL_H
#include <QAbstractTableModel>
#include "configuration.h"
#include "microprograminstruction.h"
#include "programinstructionlist.h"

class ProgramTableModel : public QAbstractTableModel
{
private:
    Configuration & c;
    ProgramInstructionList & p;
    void checkTargets();

public:
    ProgramTableModel(Configuration & conf, ProgramInstructionList & prog, QObject * parent=nullptr);

    int rowCount(QModelIndex const & parent=QModelIndex()) const;
    int columnCount(QModelIndex const & parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::DropActions supportedDropActions() const { return Qt::TargetMoveAction;}
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void addRow(MicroProgramInstruction * newInstruction);

public slots:
    void rowDropAction(int from, int to);
};

#endif // PROGRAMTABLEMODEL_H
