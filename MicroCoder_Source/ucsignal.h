#ifndef UCSIGNAL_H
#define UCSIGNAL_H

#include <QString>
#include <QList>
#include <QAbstractListModel>
#include "tokenizer.h"

// BUGBUG: Needs operator=, copy constructor
class UCSignal
{
public:
   explicit UCSignal(QString const & n, int w=1, int p=0,int r=10);
   UCSignal(UCSignal const &);
   UCSignal & operator=(UCSignal const & rhs);
   UCSignal(Tokenizer & t);
   void ChangeName(QString const & n);
   void ChangeWidth(int w);
   void AssignId(int id);
   QString Name(void) const;
   int Width(void) const;
   bool InRange(int value) const;
   void ChangePos(int p);
   int Pos(void) const;
   int Id(void) const;
   void SetRadix(int);
   int RadixInt(void) const;
   QChar RadixChar(void) const;

private:
   QString name;
   int width;
   int pos;
   // Here's my hacky solution to the problem of re-ordering.  We assign the signal ID at the first moment a new signal
   // is added to the SignalList.  After that,it can report its id, but it will never again be asked to change it.  Thus, when
   // we implement microcode for a given configuration, the microcode can carry around IDs as pointers allowing us to change
   // signal names, signal sizes and signal locations willy-nilly without breaking the underlying code.
   int signalId;
   int radix;
};

class SignalList
{
private:
   QList<UCSignal> theList;
   int nextId;
public:
   SignalList(void);
   SignalList(SignalList const &);
   SignalList & operator=(SignalList const & lhs);
   void appendNew(UCSignal const &);
   UCSignal & at(int i);
   UCSignal const & at(int i) const;
   int size(void) const;
   UCSignal removeAt(int i);
   void insertAt(UCSignal const & s, int i);
   void ReOrder(void);
   bool Has(QString const & name);
};

#endif // UCSIGNAL_H
