#include "microprograminstruction.h"

MicroProgramInstruction::MicroProgramInstruction(Configuration const & c):
   configuration(c),
   label(),
   target("NEXT"),
   condition(c.GetConditions().at(0)),
   validTarget(false),
   values()
{
}

QString MicroProgramInstruction::ParseTextOrBlank(Tokenizer & t)
{
    Token token = t.GetToken();
    QString value;
    if (token.Type() == Token::STRING)
    {
        value = token.Value();
    }
    else if (token.Type()==Token::BLANK)
    {
        value = "";
    }
    else {
        t.Throw("Unexpected token encountered");
    }
    t.NextToken();
    if (t.GetToken().Type()!=Token::COMMA)
        t.Throw("Expected comma after text");
    return value;
}

int MicroProgramInstruction::ParseSignalValue(Tokenizer &t, const UCSignal &ucs)
{
    Token token = t.GetToken();
    int numValue=0;
    if (token.Type()==Token::NUMBER)
        numValue = token.NumValue();
    else if (token.Type() != Token::STRING)
        t.Throw("Unexpected token expecting numerical value");
    else
    {
        // OK this is a prefixed string.  That means we should be able to choose the radix with the first character
        // and let QString do the rest
        int radix=0;
        QString radNumber = token.Value();
        switch(radNumber.at(0).toLatin1())
        {
        case 'D':
            radix = 10;
            break;
        case 'H':
            radix = 16;
            break;
        case 'B':
            radix = 2;
            break;
        default:
            t.Throw("Unrecognized radix in numerical value");
        }
        bool success;
        numValue = radNumber.remove(0,1).toInt(&success,radix);
        if (!success)
            t.Throw("Invalid numerical entry");
    }

    // If we got here, numValue should now contain the valid value of this numerical input.  Now to check the range
    if (!ucs.InRange(numValue))
        t.Throw("Signal value requires too many bits to encode");
    return numValue;
}

bool MicroProgramInstruction::ValidLabel(QString const & s)
{
    if ((s=="CONFIGURATION")||(s=="SIGNAL")||(s=="CONDITION")||(s=="ENDCONFIGURATION")||
         (s=="PROGRAM")||(s=="ENDPROGRAM"))
        return false;
    else
        return true;

}
MicroProgramInstruction::MicroProgramInstruction(Configuration const & c, Tokenizer & t) :
   configuration(c)
{
   // We assume the current token is the first token of the instruction.  ie: its label.
   label = ParseTextOrBlank(t);
   if (!ValidLabel(label))
       t.Throw("Labels cannot be reserved words");

   t.NextToken();

   // Next we find the branch target, which can be blank indicating fall through
   target = ParseTextOrBlank(t);
   t.NextToken();

   // Next we find the condition
   condition = ParseTextOrBlank(t);
   if ((condition == "") || (!configuration.GetConditions().contains(condition,Qt::CaseInsensitive)))
       t.Throw("Instructions must include a valid condition");
   t.NextToken();

   // OK, now we are into the data.  We will accept prefix D,H,B. No prefix means D.  The parse function will throw
   // if it is not properly formatted, so need for that.  Our job here is to check for a terminating comma.
   // Any token that is not a comma is the next line.

   int signalLoc = 0;
   for (;;)
   {
       UCSignal const & ucs = c.GetSignals().at(signalLoc);
       int value = ParseSignalValue(t,ucs);
       values[ucs.Id()]=value;

       t.NextToken();
       Token token = t.GetToken();
       signalLoc++;

       if (token.Type()==Token::COMMA)
       {
           if (signalLoc >=c.GetSignals().size())
               t.Throw("Too many values specified on line");
       }
       else if (signalLoc == c.GetSignals().size())
       {
           break;
       }
       // Eat the comma
       t.NextToken();
   }
}

int MicroProgramInstruction::Value(UCSignal const & ucs) const
{
    return values[ucs.Id()];
}

void MicroProgramInstruction::setValue(const UCSignal &ucs, int v)
{
    values[ucs.Id()]=v;
}
void MicroProgramInstruction::Write(QTextStream & stream) const
{
    QString labelString = label;
    if (label == "")
        labelString = "~";
    QString targetString = target;
    if (target == "")
        targetString = "~";
    stream << "\t" << labelString << "," << targetString << "," << condition ;

    for (int i=0;i<configuration.GetSignals().size();i++)
    {
        UCSignal const & signal = configuration.GetSignals().at(i);
        stream <<"," << signal.RadixChar() << QString::number(Value(signal),signal.RadixInt());
    }
    stream << "\n";
}

void MicroProgramInstruction::WriteOutput(QTextStream & stream, MicroInstruction &bytes, int targetValue, int radix) const
{
    // Write microprogram instruction as string of bits (microinstruction)
    bytes.SetAddressBits(targetValue);
    bytes.SetConditionBits(configuration.ConditionNumber(condition));
    int currentPos = 0;
    for (int i=0;i<configuration.GetSignals().size();i++)
    {
        UCSignal const & signal = configuration.GetSignals().at(i);
        // SetBits for the signal
        bytes.SetBits(currentPos+signal.Width()-1,currentPos,Value(signal));
        currentPos += signal.Width();
    }
    stream << bytes.OutputString(radix);
}

bool MicroProgramInstruction::operator==(const QString &label) const
{
    return this->label == label;
}
