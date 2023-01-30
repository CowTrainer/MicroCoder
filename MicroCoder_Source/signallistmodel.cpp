#include "signallistmodel.h"
#include "mimedatawrapper.h"
#include <QMessageBox>
#include <QMimeData>
#include "configuration.h"
int SignalListModel::rowCount(QModelIndex const &) const
{
   return signalList.size();
}

int SignalListModel::columnCount(QModelIndex const &) const
{
   return 2;
}

QVariant SignalListModel::data(QModelIndex const & index, int role) const
{
   if (!index.isValid())
      return QVariant();

   if (index.row() >= signalList.size())
      return QVariant();

   if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
   {
      UCSignal const & element = signalList.at(index.row());
      if (index.column() == 0)
         return element.Name();
      else if (index.column() == 1)
         return element.Width();
      else
         return QVariant();
   }
   else
      return QVariant();
}

QVariant SignalListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
   QVariant returnValue;

   if (role != Qt::DisplayRole)
       return QVariant();

   if (orientation == Qt::Horizontal)
   {
      switch(section)
      {
      case 0:
         returnValue = QString("Name");
         break;
      case 1:
         returnValue = QString("Width");
         break;
      default:
         returnValue = QVariant();
         break;
      }
      return returnValue;
   }
   else
   {
      UCSignal const & element = signalList.at(section);
      if (element.Width()==1)
         returnValue = QString("%1").arg(element.Pos());
      else
         returnValue = QString("%1..%2").arg(element.Pos()).arg(element.Pos()+element.Width()-1);
      return returnValue;
   }
}

Qt::ItemFlags 	SignalListModel::flags ( const QModelIndex & index ) const
{
   if (index.isValid())
      return Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled|Qt::ItemIsEditable;
//      return Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable;

   return Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled;
//   return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

Qt::DropActions SignalListModel::supportedDragActions() const
{
   return Qt::MoveAction|Qt::TargetMoveAction;
}

Qt::DropActions SignalListModel::supportedDropActions() const
{
   return Qt::MoveAction|Qt::TargetMoveAction;
}



QMimeData *SignalListModel::mimeData(const QModelIndexList &indexes) const
{
/*   if (indexes.size() != 1)
   {
       QMessageBox msgBox;
       QString string = QString("%1 indices").arg(indexes.size());
       msgBox.setText("Drop of multiple signals is not supported. Only the first will be dropped");
       msgBox.exec();
   }*/

   int row = indexes[0].row();
   UCSignal ucs = signalList.at(row);
   QMimeData *mimeData = new MimeDataWrapper(ucs);
   QByteArray encodedData;
   encodedData.append(row & 0xFF);
   mimeData->setData("application/ucsignal",encodedData);
   return mimeData;
}

QStringList SignalListModel::mimeTypes() const
{
   QStringList typeList;
   typeList.append(QString("application/ucsignal"));
   return typeList;

}
bool SignalListModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   if (index.column() == 0)
   {
      QString newName = value.toString();
      if (!Configuration::CleanName(newName))
      {
         QMessageBox msg;
         msg.setText("Signal name contains a reserved word.");
         msg.exec();
      }

      if (signalList.Has(newName))
      {
         if (signalList.at(index.row()).Name() != newName)
         {
            QMessageBox msg;
            msg.setText("Signals must have unique names.");
            msg.exec();
         }
         return true;
      }
      signalList.at(index.row()).ChangeName(newName);
      return true;
   }
   if (index.column() == 1)
   {
      signalList.at(index.row()).ChangeWidth(value.toInt());
      signalList.ReOrder();
      beginResetModel();
      endResetModel();
      emit headerDataChanged(Qt::Vertical,0,signalList.size()-1);
      return true;
   }
}


bool SignalListModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int targetRow, int column, const QModelIndex &parent)
{

   QStringList theList=data->formats();
   for (int i=0;i<theList.size();i++)
   {
      if (theList[i]=="application/ucsignal")
      {
         int sourceRow = data->data(theList[i]).at(0);
         // We need to verify some things about the drop location
         if ((sourceRow == targetRow) || (sourceRow+1==targetRow))
            return false;  // Means trying to drop it in the same place
         if ((sourceRow == signalList.size()-1) && (targetRow==-1))
            return false;  // Trying to drop the last row at the end
         if (targetRow == -1)
            targetRow = signalList.size();//The invisible space at the end is the same as the end

         const MimeDataWrapper * myData =
                     qobject_cast<const MimeDataWrapper *>(data);
         UCSignal ucs = myData->S();
         beginMoveRows(parent, sourceRow,sourceRow,parent,targetRow);
         signalList.removeAt(sourceRow);
         signalList.insertAt(ucs,targetRow);
         endMoveRows();
      }
   }
   return true;
}

void SignalListModel::AddSignal(UCSignal const & s)
{
   QString newName = s.Name();
   if (!Configuration::CleanName(newName))
   {
      QMessageBox msg;
      msg.setText("Signal names cannot contain reserved words");
      msg.exec();
      return;
   }

   if (signalList.Has(newName))
   {
      QMessageBox msg;
      msg.setText("Signals must have unique names.");
      msg.exec();
      return;
   }
   beginInsertRows(QModelIndex(),signalList.size(),signalList.size());
   signalList.appendNew(s);
   signalList.at(signalList.size()-1).ChangeName(newName);
   endInsertRows();
}

void SignalListModel::DeleteSelection(QModelIndexList const & indices)
{
   int first = indices.first().row();
   int last = indices.last().row();
   if (signalList.size() == 0)
      return;
   beginRemoveRows(indices.first().parent(),first,last);
   for (int i=first;i<=last;i++)
      signalList.removeAt(i);
   endRemoveRows();
}
