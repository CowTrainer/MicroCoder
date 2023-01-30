#include "stringspindelegate.h"
#include <QListView>
#include <QSpinBox>

CondSpinDelegate::CondSpinDelegate(Configuration const & c,QObject *parent)
    : QStyledItemDelegate(parent),conf(c)
{
    pModel = new MyStringListModel(conf.GetConditions(),this,true);
}

QWidget *CondSpinDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    // Fix not displaying on first click
    ListComboBox * editor = new ListComboBox(parent);
    editor->setModel(pModel);

    return editor;
}

void CondSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    ListComboBox *theBox = static_cast<ListComboBox *>(editor);
    theBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
}

void CondSpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    ListComboBox *lv = static_cast<ListComboBox*>(editor);;
    QString selected =lv->currentText();
    model->setData(index, selected, Qt::EditRole);
}
void CondSpinDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

ListComboBox::ListComboBox(QWidget *parent)
    : QComboBox(parent)
{}

void ListComboBox::focusInEvent(QFocusEvent *e)
{
    QComboBox::focusInEvent(e);
}
