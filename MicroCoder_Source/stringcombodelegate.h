#ifndef STRINGCOMBODELEGATE_H
#define STRINGCOMBODELEGATE_H

#include "programinstructionlist.h"
#include <QObject>
#include <QStyleOptionViewItem>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QValidator>
#include "QStringListModel"
#include <QCompleter>
#include <QComboBox>
#include <QLineEdit>

// Dummy validator
class Validator : public QValidator
{
public:
    State validate(QString &input, int &pos ) const
    {
        Q_UNUSED(input)
        Q_UNUSED(pos)
        return QValidator::Acceptable;
    }
};

class MyComboBox : public QComboBox
{
public:
    explicit MyComboBox(QWidget *parent = nullptr);
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *e) override;
};

class TargetComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ProgramInstructionList const & prog;
    QStringListModel * pModel;

public:
    explicit TargetComboDelegate(ProgramInstructionList const & p, QObject *parent = 0);

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


#endif // STRINGCOMBODELEGATE_H
