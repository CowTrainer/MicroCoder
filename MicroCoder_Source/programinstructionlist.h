#ifndef PROGRAMINSTRUCTIONLIST_H
#define PROGRAMINSTRUCTIONLIST_H

#include "microprograminstruction.h"
#include <QList>
#include <QMap>
#include <QString>

// Class Intended to store microprograminstructions and preform
// basic functions such as checking for a duplicate and such
class ProgramInstructionList
{

private:
    QList<MicroProgramInstruction *> program;
    // Map instead of set so stores in order
    QMap<QString,int> labels;
    void InsertLabel (const QString & label);
public:
    ProgramInstructionList();
    void AddInstruction(MicroProgramInstruction *inst);
    void AddInstructionAt(MicroProgramInstruction *inst, int index);
    void MoveInstruction(int oldIndex, int newIndex);
    void SwapInstructions(int firstIndex, int secondIndex);
    void DeleteInstruction (int index);
    void EraseProgram ();
    MicroProgramInstruction* InstructionAt (int index) {return program.at(index);}
    bool IsEmpty() const {return program.isEmpty();}
    bool LabelExists (const QString & label) const {return labels.contains(label);}
    QList<QString> GetLabels() const {return labels.keys();}
    bool ValidTarget (const QString & target, int index) const;
    bool ErrorExists () const;
    // Here is my hacky solution to changing labels.
    // While you can use the inclass one, the in list one is going to
    // be preferred 99% of the time due to the fact that it checks for
    // new valid ones
    void SetLabelAt (int index, const QString & newLabel);
    void SetTargetAt (int index, const QString & newTarget);
    void CheckTargets ();
    void SetValueAt(const UCSignal &ucs, int index, int v);
    int Size () {return program.size();};
};

#endif // PROGRAMINSTRUCTIONLIST_H
