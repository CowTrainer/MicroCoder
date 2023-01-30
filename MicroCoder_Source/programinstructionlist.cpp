#include "programinstructionlist.h"

ProgramInstructionList::ProgramInstructionList():
    program(),
    labels()
{
}

// Adds instruction without checking
void ProgramInstructionList::AddInstruction(MicroProgramInstruction *inst)
{
    program.push_back(inst);
    InsertLabel(inst->Label());
}

// Adds instruction at a certain line with checking
void ProgramInstructionList::AddInstructionAt(MicroProgramInstruction *inst, int index)
{
    program.insert(index+1, inst);
    InsertLabel(inst->Label());
    CheckTargets();
}

// Moves an item from one location to another and check (I really love qt list default functions)
void ProgramInstructionList::MoveInstruction(int oldIndex, int newIndex)
{
    program.move(oldIndex,newIndex);
    CheckTargets();
}



void ProgramInstructionList::SwapInstructions(int firstIndex, int secondIndex)
{
    program.swapItemsAt(firstIndex, secondIndex);
    // Also maybe speed issue here
    CheckTargets();
}

void ProgramInstructionList::DeleteInstruction(int index)
{
    // Implement check
    labels.remove(program.at(index)->Label());
    delete program.at(index);
    program.removeAt(index);
    CheckTargets();
}

void ProgramInstructionList::EraseProgram()
{
    for (int i=0;i<program.size();i++)
        delete program.at(i);
    program.clear();
    labels.clear();
}

bool ProgramInstructionList::ValidTarget(const QString & target, int index) const
{
    if(labels.contains(target))
    {
        return true;
    }
    else if (target == "NEXT")
    {
        // if the next microinstruction is not the end of the program (or bigger,
        // if it is bigger something has gone horribly wrong
        if(!(index+1 >= program.size()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool ProgramInstructionList::ErrorExists() const
{
    for(int i=0;i<program.size();i++)
    {
        if(!(program.at(i)->ValidTarget()))
        {
            return true;
        }
    }
    return false;
}

void ProgramInstructionList::SetLabelAt(int index, const QString & newLabel)
{
    MicroProgramInstruction * inst =  program.at(index);
    if (newLabel != inst->Label())
    {
        labels.remove(inst->Label());
        InsertLabel(newLabel);
        inst->SetLabel(newLabel);

        CheckTargets();
    }
}

void ProgramInstructionList::SetTargetAt(int index, const QString & newTarget)
{
    MicroProgramInstruction * inst =  program.at(index);
    if (newTarget != inst->Target())
    {
        inst->SetTarget(newTarget);
        if (ValidTarget(newTarget, index))
        {
            inst->SetValidTarget(true);
        }
        else
        {
            inst->SetValidTarget(false);
        }
    }
}

void ProgramInstructionList::InsertLabel(const QString & label)
{
    // Don't want blank labels!!! In our set atleast
    if(!(label == ""))
    {
        labels.insert(label,0);
    }
}

void ProgramInstructionList::CheckTargets()
{
    for(int i=0;i<program.size();i++)
    {
        if(labels.contains(program.at(i)->Target()))
        {
            program.at(i)->SetValidTarget(true);
        }
        else if((program.at(i)->Target()) == "NEXT" && (i != program.size()-1))
        {
            program.at(i)->SetValidTarget(true);
        }
        else
        {
            program.at(i)->SetValidTarget(false);
        }
    }
}

void ProgramInstructionList::SetValueAt(const UCSignal &ucs, int index, int v)
{
    program[index]->setValue(ucs,v);
}

