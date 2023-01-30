#ifndef MICROINSTRUCTION_H
#define MICROINSTRUCTION_H
#include <QByteArray>
#include <QString>

class MicroInstruction
{
private:
   QByteArray bytes;
   int addressBits;
   int conditionBits;
   int signalBits;
public:
   MicroInstruction(int addressBits,int conditionBits,int signalBits);
   void SetAddressBits(int address);
   void SetBits(int rangeHi, int rangeLo, int value);
   void SetConditionBits(int condition);
   QString ToHex(void) const;
   QString OutputString(int radix) const;
   int Width(void) const {return bytes.size();}
   unsigned long BuildUpWord(int rangeHi, int rangeLo) const;
   QString BuildUpHex(int rangeHi, int rangeLo) const;
};

#endif // MICROINSTRUCTION_H
