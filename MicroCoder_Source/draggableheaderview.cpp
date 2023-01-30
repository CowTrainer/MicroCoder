#include <QMouseEvent>
#include <QDrag>

#include "draggableheaderview.h"

DraggableHeaderView::DraggableHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
     connect(this, &DraggableHeaderView::sectionMoved, this, &DraggableHeaderView::onSectionMoved);
}

// When section moved in header view, reverse the change and signify for real change
// Incredibly hacky solution, block siganls to prevent circular loop, then allowed after moveSection is done
void DraggableHeaderView::onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex){
    Q_UNUSED(logicalIndex)
    blockSignals(true);
    moveSection(newVisualIndex, oldVisualIndex);
    blockSignals(false);
    emit instructionMove(newVisualIndex,oldVisualIndex);
}
