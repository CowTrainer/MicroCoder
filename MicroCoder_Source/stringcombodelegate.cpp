#include "stringcombodelegate.h"
#include "qevent.h"
#include <algorithm>
#include <QList>
#include <QListView>
#include <QLineEdit>
#include <QComboBox>
#include <QCompleter>
#include <QDebug>


TargetComboDelegate::TargetComboDelegate(ProgramInstructionList const & p,QObject *parent)
    : QStyledItemDelegate(parent),prog(p)
{
    pModel = new QStringListModel(p.GetLabels(),parent);
}

QWidget *TargetComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)
    MyComboBox * editor = new MyComboBox(parent);
    QStringList labels = prog.GetLabels();
    labels.prepend("NEXT");
    pModel->setStringList(labels);
    editor->setEditable(true);
    editor->completer()->setCompletionMode(QCompleter::PopupCompletion);
    editor->setInsertPolicy(QComboBox::NoInsert);
    // Not sure about this either
    editor->setValidator(new Validator());
    editor->setModel(pModel);
    return editor;
}

void TargetComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    MyComboBox *theBox = static_cast<MyComboBox *>(editor);
    theBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
}

void TargetComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    MyComboBox *lv = static_cast<MyComboBox*>(editor);;
    QString selected =lv->currentText();
    model->setData(index, selected, Qt::EditRole);
}
void TargetComboDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(editor)
    editor->setGeometry(option.rect);
}

MyComboBox::MyComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void MyComboBox::focusInEvent(QFocusEvent *e)
{
    QComboBox::focusInEvent(e);
    lineEdit()->completer()->setCompletionPrefix("");
    completer()->complete();
    lineEdit()->setFocus();
}

void MyComboBox::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return)
    {
        QComboBox::keyPressEvent(e);
        QFocusEvent *event = new QFocusEvent(QEvent::FocusOut, Qt::FocusReason(0));
        QComboBox::focusOutEvent(event);
        QComboBox::focusOutEvent(event);
    }
    else
    {
        QComboBox::keyPressEvent(e);
    }

}

// Put in edit text changed
