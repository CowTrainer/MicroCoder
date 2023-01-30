#include "ucsignal.h"
#include <QMessageBox>

UCSignal::UCSignal(QString const & n, int w, int p,int r) :
    name(n.toUpper()),
    width(w),
    pos(p),
    signalId(-1),
    radix(r)
{
}

void UCSignal::SetRadix(int r)
{
    radix = r;
}

UCSignal::UCSignal(UCSignal const & ucs)
{
   if (this == &ucs)
      return;

   *this = ucs;
}


UCSignal & UCSignal::operator=(UCSignal const & rhs)
{
   name = rhs.name;
   width = rhs.width;
   pos = rhs.pos;
   signalId = rhs.signalId;
   radix = rhs.radix;

   return *this;
}

UCSignal::UCSignal(Tokenizer & t) :
   signalId(-1)
{
   //Eat the word "SIGNAL"
   t.NextToken();

   // The next token better be the name.
   Token token = t.GetToken();
   if (token.Type()!=Token::STRING)
      t.Throw("Error in file. Expected signal name");
   name = token.Value().toUpper();
   t.NextToken();

   // The next token better be a comma
   if (t.GetToken().Type()!=Token::COMMA)
      t.Throw("Comma missing in signal definition");
   t.NextToken();

   // The next token better be the position
   token = t.GetToken();
   if ((token.Type()!=Token::NUMBER) ||(token.NumValue()<0))
      t.Throw("Found invalid value for signal position");
   pos = token.NumValue();
   t.NextToken();

   // The next token better be a comma
   token = t.GetToken();
   if (token.Type() != Token::COMMA)
      t.Throw("Found unexpected character instead of comma");
   t.NextToken();

   // The next token better be the width
   token=t.GetToken();
   if ((token.Type()!=Token::NUMBER) ||(token.NumValue()<0))
      t.Throw("Found invalid value for signal width");
   width = token.NumValue();
   t.NextToken();

   // The next token better be a comma
   token=t.GetToken();
   if (token.Type() != Token::COMMA)
      t.Throw("Found unexpected character instead of comma");
   t.NextToken();

    token=t.GetToken();
   if (token.Type()!=Token::NUMBER)
       t.Throw("Found unexpected token expecting radix");
   radix = token.NumValue();
   if ((radix!=2) && (radix!=10)&&(radix!=16))
       t.Throw("Only base 2, base 10 and base 16 are permitted");

   // What happens next is not our problem.  We do not advance to the next token.
   // that is the caller's job.
}

void UCSignal::ChangeName(QString const & n)
{
    name = n.toUpper();
}

void UCSignal::ChangeWidth(int w)
{
    width = w;
}
void UCSignal::ChangePos(int p)
{
   pos = p;
}

void UCSignal::AssignId(int id)
{
   if (signalId >= 0)
   {
      QMessageBox msgBox;
      msgBox.setText("Somebody is attempting to re-assign a signal Id");
      msgBox.exec();
      return;
   }
   signalId = id;
}

QString UCSignal::Name(void) const
{
    return name;
}

int UCSignal::Width(void) const
{
    return width;
}
bool UCSignal::InRange(int numValue) const
{
    if (numValue < 0)
        return false;
    if (numValue > (1<<width)-1)
        return false;
    return true;
}
int UCSignal::Pos(void) const
{
   return pos;
}
int UCSignal::Id(void) const
{
   return signalId;
}

int UCSignal::RadixInt() const
{
    return radix;
}

QChar UCSignal::RadixChar() const
{
    switch(radix)
    {
    case 2:
        return 'B';
    case 16:
        return 'H';
    case 10:
    default:
        return 'D';
    }
}

SignalList::SignalList(void)
   : nextId(0)
{
   ;
}

SignalList::SignalList(SignalList const &s)
{
   *this = s;
}

SignalList & SignalList::operator=(SignalList const & lhs)
{
   if (this == &lhs)
      return *this;

   theList.clear();
   for (int i=0;i<lhs.size();i++)
   {
      theList.append(lhs.at(i));
   }
   nextId = lhs.nextId;
   return *this;
}

int SignalList::size(void) const
{
   return theList.size();
}
bool SignalList::Has(QString const & name)
{
   for (int i=0;i<theList.size();i++)
   {
      if (name.toUpper() == theList.at(i).Name())
      {
         return true;
      }
   }
   return false;
}

void SignalList::appendNew(UCSignal const & s)
{
   UCSignal newS(s);
   if (theList.size()==0)
   {
      newS.ChangePos(0);
   }
   else
      newS.ChangePos(theList.last().Pos()+theList.last().Width());

   newS.AssignId(nextId++);
   theList.append(newS);
}

UCSignal & SignalList::at(int i)
{
   return theList[i];
}

UCSignal const & SignalList::at(int i) const
{
   return theList.at(i);
}

UCSignal SignalList::removeAt(int i)
{

   UCSignal rv=theList.at(i);
   theList.removeAt(i);
   ReOrder();
   return rv;
}


// Note that the notion of a signalId means it is NEVER possible to insert a new UCSignal into the middle of
// a list.  New signals must be appended.
void SignalList::insertAt(UCSignal const & s, int i)
{
   if (s.Id() < 0)
   {
      QMessageBox msgBox;
      msgBox.setText("Attempt to insert a new UCSignal instead of appending");
      msgBox.exec();
   }
   theList.insert(i,s);
   ReOrder();
}

void SignalList::ReOrder(void)
{
   if (theList.size() == 0)
      return;

   theList.first().ChangePos(0);
   for (int i=1;i<theList.size();i++)
   {
      theList[i].ChangePos(theList[i-1].Pos()+theList[i-1].Width());
   }
}

