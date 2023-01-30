#include "binaryspindelegate.h"
#include <QSpinBox>
#include <QAbstractItemModel>

BinarySpinDelegate::BinarySpinDelegate(int mn,int mx, int rx, QObject *parent) :
   QStyledItemDelegate(parent), min(mn),max(mx),radix(rx)
{
}

QWidget *BinarySpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
   QSpinBox * editor = new QSpinBox(parent);
   editor->setFrame(false);
   editor->setMinimum(min);
   editor->setMaximum(max);
   editor->setDisplayIntegerBase(radix);
   return editor;
}

void BinarySpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   int value = index.model()->data(index, Qt::EditRole).toInt();
   QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
   spinBox->setValue(value);
}

void BinarySpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
   QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
   spinBox->interpretText();
   int value = spinBox->value();
   model->setData(index, value, Qt::EditRole);
}
void BinarySpinDelegate::updateEditorGeometry(QWidget *editor,
                                              const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   editor->setGeometry(option.rect);
}
