#ifndef STRINGSPINDELEGATE_H
#define STRINGSPINDELEGATE_H

#include "configuration.h"
#include <QObject>
#include <QStyleOptionViewItem>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QComboBox>
#include "mystringlistmodel.h"

class ListComboBox : public QComboBox
{
public:
    explicit ListComboBox(QWidget *parent = nullptr);
protected:
    virtual void focusInEvent(QFocusEvent *e) override;
};

class CondSpinDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Configuration const & conf;
    MyStringListModel * pModel;


public:
    explicit CondSpinDelegate(Configuration const & c,QObject *parent = 0);

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

#endif // STRINGSPINDELEGATE_H
