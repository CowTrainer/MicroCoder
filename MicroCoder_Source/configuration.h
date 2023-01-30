#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>
#include <ucsignal.h>
#include "microinstruction.h"
#include <QTextStream>
#include "tokenizer.h"

class Configuration      
{
private:
   SignalList theSignals;
   QStringList theConditions;
public:
   Configuration();
   Configuration(Tokenizer & t);
   Configuration(Configuration const &);
   Configuration & operator=(Configuration const & rhs);
   SignalList const & GetSignals(void) const {return theSignals;}
   SignalList & GetSignals(void) {return theSignals;}
   void SetSignals(SignalList const & sl);
   QStringList const & GetConditions(void) const {return theConditions;}
   void SetConditions(QStringList const & c);
   void PopulateSignal(MicroInstruction & mi, UCSignal const & ucs,int value) const;
   void PopulateCondition(MicroInstruction & mi, QString const & condition) const;
   UCSignal const & SignalById(int signalId) const;
   bool SignalIdExists(int signalId) const;
   int ConditionNumber(QString const & condition) const;
   bool ConditionExists(QString const & condition) const;
   void Write(QTextStream & stream) const;
   bool Empty(void) const{return theSignals.size()==0 && theConditions.size()==0;}
   static bool CleanName(QString &);
};

#endif // CONFIGURATION_H
