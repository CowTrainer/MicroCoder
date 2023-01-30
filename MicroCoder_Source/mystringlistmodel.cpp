#include "mystringlistmodel.h"
#include "configuration.h"
QVariant MyStringListModel::data(QModelIndex const & index, int role) const
{
   if (!index.isValid())
      return QVariant();

   if (index.row() >= theList.size())
      return QVariant();

   if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
   {
      if (index.column() == 0)
         return theList.at(index.row());
      else
         return QVariant();
   }
   else
      return QVariant();

}

QStringList MyStringListModel::mimeTypes() const
{
   QStringList types;
   types << "application/vnd.text.list";
   return types;
}

QMimeData * MyStringListModel::mimeData(const QModelIndexList &indexes) const
{
   int row = indexes.first().row();
   QString text = theList.at(row);
   QByteArray bytes;
   bytes.append(text);
   QMimeData * newMimeData = new QMimeData;
   newMimeData->setData("application/vnd.text.list",bytes);
   return newMimeData;
}

Qt::DropActions MyStringListModel::supportedDragActions() const
{
   return Qt::MoveAction|Qt::TargetMoveAction;
}

Qt::DropActions MyStringListModel::supportedDropActions() const
{
   return Qt::MoveAction|Qt::TargetMoveAction;
}
bool MyStringListModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int targetRow,
                                     int column, const QModelIndex & parent)
{
   Q_UNUSED(action)
   Q_UNUSED(column)
   if (data->hasFormat("application/vnd.text.list"))
   {
      QByteArray encodedData = data->data(data->formats().first());
      QString theText(encodedData);
      int sourceRow = theList.indexOf(theText);

      if ((sourceRow == targetRow) || (sourceRow+1==targetRow))
         return false;  // Means trying to drop it in the same place
      if ((sourceRow == theList.size()-1) && (targetRow==-1))
         return false;  // Trying to drop the last row at the end
      if (targetRow == -1)
         targetRow = theList.size();//The invisible space at the end is the same as the end
      beginMoveRows(parent, sourceRow,sourceRow,parent,targetRow);
      theList.move(sourceRow,targetRow);
      endMoveRows();

      return true;
   }
   return false;
}

bool MyStringListModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
   Q_UNUSED(role)
   // First, check to see if anybody's used this label yet.
   QString testString = value.toString();

   // An attempt to "set" this index to itself is not interesting
   if (testString == theList[index.row()])
      return true;

   // No idea what this does - Herry
   for (int i=0;theList.contains(testString,Qt::CaseInsensitive);i++)
   {
      testString = value.toString()+QString("%1").arg(i);
   }
   if (Configuration::CleanName(testString))
   {
      theList[index.row()] = testString;
   }
   else
   {
      QMessageBox msg;
      msg.setText("Label contains a reserved word.");
      msg.exec();
   }
   return true;
}

void MyStringListModel::DeleteSelection(QModelIndexList const & indices)
{
   int first = indices.first().row();
   int last = indices.last().row();
   if (theList.size() == 0)
      return;
   beginRemoveRows(indices.first().parent(),first,last);
   for (int i=first;i<=last;i++)
      theList.removeAt(i);
   endRemoveRows();
}

void MyStringListModel::AddNew()
{
   QString newLabel("LABEL");
   QString trial(newLabel);
   for(int i=0;theList.contains(trial,Qt::CaseInsensitive);i++)
   {
      trial = newLabel;
      trial.append(QString("%1").arg(i));
   }
   beginInsertRows(QModelIndex(),theList.size(),theList.size());
   theList.append(trial);
   endInsertRows();
}

QVariant MyStringListModel::headerData(int section, Qt::Orientation orientation,
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
      }
      return returnValue;
   }
   else
   {
      returnValue = QString("%1").arg(section);
      return returnValue;
   }
}
