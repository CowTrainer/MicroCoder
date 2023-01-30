#ifndef SIGNALLISTMODEL_H
#define SIGNALLISTMODEL_H

#include <QAbstractListModel>
#include "ucsignal.h"

class SignalListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SignalListModel(SignalList const & s, QObject *parent = 0)
        : QAbstractListModel(parent), signalList(s) {}
    virtual ~SignalListModel(){}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags 	flags ( const QModelIndex & index ) const;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    void AddSignal(UCSignal const & s);
    void DeleteSelection(QModelIndexList const & indices);
    SignalList const & GetSignals(void) const {return signalList;}

private:
    SignalList signalList;
};
#endif // SIGNALLISTMODEL_H
