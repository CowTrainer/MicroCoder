#ifndef MICROPROGRAMINSTRUCTION_H
#define MICROPROGRAMINSTRUCTION_H
#include "configuration.h"
#include "ucsignal.h"
#include <QList>

class MicroProgramInstruction
{
private:
   // Keep a reference to the current configuration.  We can't change it (hence const) but it might change on us
   Configuration const & configuration;

   QString label;   // Of this instruction.  Can be blank
   QString target;  // Of this instruction.  Defaults to fall through
   QString condition;

   bool validTarget;

   // The search key here is the ucSignalID.  In that way, we don't need to re-sort the instruction if the configuration changes
   // Note that when reading from a file, the instruction needs to interrogate the signal list to figure out which is which.
   // In the table-view mode, the assignment is column ID to signal ID.  Actual location of columns is not as important.
   QMap<int,int> values;
public:
   MicroProgramInstruction(Configuration const & c);
   MicroProgramInstruction(Configuration const & c, Tokenizer & t);
   QString Label(void) const {return label;}
   // Dont use this SetLabel (mostly) it won't update valid/invalid targets!!!
   void SetLabel(QString const & s) {label = s;}
   QString Target(void) const {return target;}
   void SetTarget(QString const & s){target = s;}
   QString Condition(void) const {return condition;}
   void SetCondition(QString const & s){condition = s;}
   int Value(UCSignal const & ucs) const;
   void setValue(UCSignal const & ucs,int v);
   bool ValidTarget(void) const {return validTarget;}
   void SetValidTarget(bool s) {validTarget = s;}
   void Write(QTextStream & stream) const;
   void WriteOutput(QTextStream & stream, MicroInstruction &bytes, int targetValue, int radix) const;
   static QString ParseTextOrBlank(Tokenizer & t);
   static int ParseSignalValue(Tokenizer & t, UCSignal const & ucs);
   bool operator==(QString const & label) const;
   static bool ValidLabel(QString const & s);

};

#endif // MICROPROGRAMINSTRUCTION_H
