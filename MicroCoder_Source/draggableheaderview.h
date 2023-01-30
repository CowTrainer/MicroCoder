#ifndef DRAGGABLEHEADERVIEW_H
#define DRAGGABLEHEADERVIEW_H

#include <QHeaderView>

class DraggableHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit DraggableHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

private slots:
    void onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);

signals:
    void instructionMove(int to, int from);

};

#endif // GENERATEDLG_H
