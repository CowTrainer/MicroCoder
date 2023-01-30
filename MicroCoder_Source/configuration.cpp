#include "configuration.h"
#include <QTextStream>

Configuration::Configuration()
{
}

Configuration::Configuration(Configuration const & c)
{
   *this = c;
}

Configuration & Configuration::operator=(Configuration const & rhs)
{
   if (this == &rhs)
      return *this;
   theSignals = rhs.theSignals;
   theConditions = rhs.theConditions;
   return *this;
}

Configuration::Configuration(Tokenizer &t)
{
   Token token = t.GetToken();
   if ((token.Type() != Token::STRING) || (token.Value() != "CONFIGURATION"))
      t.Throw("The first token in the file must be reserved word CONFIGURATION");
   // Eat CONFIGURATION
   t.NextToken();

   while(t.GetToken().Type()!=Token::ENDOFFILE)
   {
      Token token=t.GetToken();
      if (token.Value() == "SIGNAL")
      {
         UCSignal newSignal(t);
         if (theSignals.Has(newSignal.Name()))
            t.Throw("File has been modified and rendered invalid.  Signals must have unique names.");
         theSignals.appendNew(newSignal);
      }
      else if (token.Value()== "CONDITION")
      {
         t.NextToken();
         token = t.GetToken();
         if (theConditions.contains(token.Value()))
            t.Throw("File has been modified and rendered invalid.  Conditions must have unique names.");
         theConditions.append(token.Value());
      }
      else if (token.Value()=="ENDCONFIGURATION")
         return;  // Remember.  Always the caller's job to advance past the last interesting token
      else
          t.Throw("Unrecognized token in configuration");
      t.NextToken();
   }
   t.Throw("Unexpected end of file found while parsing CONFIGURATION");
}

void Configuration::SetSignals(SignalList const & sl)
{
   theSignals = sl;
}

void Configuration::SetConditions(QStringList const & c)
{
   theConditions = c;
}

void Configuration::PopulateSignal(MicroInstruction & mi, UCSignal const & ucs,int value) const
{
   mi.SetBits(ucs.Pos()+ucs.Width()-1,ucs.Pos(),value);
}

void Configuration::PopulateCondition(MicroInstruction & mi, QString const & condition) const
{
   int conditionNumber = theConditions.indexOf(condition);
   mi.SetConditionBits(conditionNumber);
}

bool Configuration::SignalIdExists(int signalId) const
{
   for (int i=0;i<theSignals.size();i++)
   {
      if (signalId == theSignals.at(i).Id())
         return true;
   }
   return false;
}

UCSignal const & Configuration::SignalById(int signalId) const
{
   for (int i=0;i<theSignals.size();i++)
   {
      if (theSignals.at(i).Id() == signalId)
         return theSignals.at(i);
   }
   throw QString("Attempt to use deleted signal.");
}

int Configuration::ConditionNumber(QString const & condition) const
{
   return theConditions.indexOf(condition.toUpper());
}
bool Configuration::CleanName(QString &inout)
{
   if (inout.contains(';'))
      inout.replace(";",":");
   if (inout.contains(','))
      inout.replace(",",".");
   if (inout.contains(' '))
      inout.replace(" ","_");
   if (inout.contains("#"))
      inout.replace("#","_");
   if ((inout.contains("CONFIGURATION")||inout.contains("SIGNAL")||inout.contains("CONDITION")||inout.contains("ENDCONFIGURATION")||
        inout.contains("PROGRAM")||inout.contains("ENDPROGRAM")))
      return false;
   inout = inout.toUpper();
   return true;
}

bool Configuration::ConditionExists(QString const & condition) const
{
   return theConditions.contains(condition,Qt::CaseInsensitive);
}

void Configuration::Write(QTextStream & stream) const
{
   stream << "CONFIGURATION\n";

   for (int i=0;i<theConditions.size();i++)
   {
      stream << "\t" <<"CONDITION " << theConditions.at(i) << "\n";
   }
   for (int i=0;i<theSignals.size();i++)
   {
      UCSignal const & ucs = theSignals.at(i);
      stream << "\t" <<"SIGNAL " << ucs.Name() << "," << ucs.Pos() << "," << ucs.Width() << "," << ucs.RadixInt() << "\n";
   }
   stream << "ENDCONFIGURATION\n";
}
