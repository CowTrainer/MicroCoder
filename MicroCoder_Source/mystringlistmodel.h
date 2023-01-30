#ifndef MYSTRINGLISTMODEL_H
#define MYSTRINGLISTMODEL_H

#include <QObject>
#include <QStringList>
#include <QAbstractListModel>
#include <QMimeData>
#include <QMessageBox>

class MyStringListModel : public QAbstractListModel
{
   Q_OBJECT

private:
    bool readOnly;
   QStringList theList;
public:
   MyStringListModel(QStringList const & l, QObject *parent,bool ro=false)
       : QAbstractListModel(parent),readOnly(ro),theList(l){}
   Qt::ItemFlags 	flags ( const QModelIndex & index ) const
   {
      if (index.isValid() && !readOnly)
         return Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled|Qt::ItemIsEditable;
      if (index.isValid() && readOnly)
        return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
      return Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled;
   }
   bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
   bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
   QStringList mimeTypes() const;
   Qt::DropActions supportedDragActions() const;
   Qt::DropActions supportedDropActions() const;
   QMimeData *mimeData(const QModelIndexList &indexes) const;
   QStringList stringList(void) const {return theList;}
   int rowCount(const QModelIndex& i = QModelIndex()) const {Q_UNUSED(i) return theList.size();}
   int columnCount(const QModelIndex & parent = QModelIndex()) const {Q_UNUSED(parent) return 1;}
   QVariant data(const QModelIndex &, int role) const;
   QVariant headerData(int section, Qt::Orientation orientation,
                                          int role) const;
   void DeleteSelection(QModelIndexList const & indices);
   void AddNew(void);
};

#endif // MYSTRINGLISTMODEL_H
