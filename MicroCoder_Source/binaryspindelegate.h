#ifndef BINARYSPINDELEGATE_H
#define BINARYSPINDELEGATE_H

#include <QObject>
#include <QStyleOptionViewItem>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>

class BinarySpinDelegate : public QStyledItemDelegate
{
   Q_OBJECT
   int min;
   int max;
   int radix;
public:
   explicit BinarySpinDelegate(int min,int max,int radix=10,QObject *parent = 0);

   QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;
   void setEditorData(QWidget *editor, const QModelIndex &index) const;
   void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const;
   void updateEditorGeometry(QWidget *editor,
                             const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // BINARYSPINDELEGATE_H
