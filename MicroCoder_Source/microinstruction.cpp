#include "microinstruction.h"

MicroInstruction::MicroInstruction(int a,int c,int s)
    : bytes(a+c+s,0),addressBits(a),conditionBits(c),
      signalBits(s)
{
}

void MicroInstruction::SetAddressBits(int address)
{
   SetBits(bytes.size()-1,conditionBits+signalBits,address);
}

void MicroInstruction::SetConditionBits(int condition)
{
   SetBits(conditionBits+signalBits-1,signalBits,condition);
}

void MicroInstruction::SetBits(int rangeHi, int rangeLo, int value)
{
   int exponent = rangeHi-rangeLo;
   unsigned int mask =(1<<exponent);
   unsigned int uvalue = (unsigned int)value;

   // Binary is read right to left, lower index values worth higher in array
   // Not sure about this however
   for (int i=rangeLo;i<=rangeHi;i++,mask = mask>>1)
   {
      bytes[i] = bool(uvalue & mask);
   }
}

QString MicroInstruction::ToHex(void) const
{
   unsigned long temp;
   int ulongSize = sizeof(temp)*8;
   int highestOrderWordSize = bytes.size()%ulongSize;
   int fullDoubleWords = bytes.size()/ulongSize;

   // Create own ToHexString with build up wrod
   QString returnValue=BuildUpHex(highestOrderWordSize-1,0);

   for (int word=1;word <= fullDoubleWords;word++)
   {
      returnValue.append(BuildUpHex(highestOrderWordSize+word*ulongSize,highestOrderWordSize+(word-1)*ulongSize));
   }
   return returnValue;
}

QString MicroInstruction::OutputString(int radix) const
{
    if(radix == 16)
    {
        return ToHex();
    }
    if (radix == 10)
    {
        return QString::number(BuildUpWord(bytes.size()-1,0));
    }
    if (radix == 2)
    {
        QString temp = "";
        for (int i = 0; i < bytes.size(); i++)
        {
            if(bytes[i])
                temp.append('1');
            else
                temp.append('0');
        }
        return temp;
    }
    return "";
}

unsigned long MicroInstruction::BuildUpWord(int rangeHi, int rangeLo) const
{
   unsigned long returnValue = 0;
   for (int bit = rangeHi-rangeLo;bit>=0;bit--)
      returnValue |= (bytes[rangeHi-bit])<<bit;
   return returnValue;
}

QString MicroInstruction::BuildUpHex(int rangeHi, int rangeLo) const
{
    int highestOrderNibble = (rangeHi-rangeLo+1)%4;
    int fullNibbles = (rangeHi-rangeLo-highestOrderNibble+1)/4;
    QString returnValue = "";
    if (highestOrderNibble>1)
        returnValue.append((QString("%1").arg(BuildUpWord(highestOrderNibble-1,0),0,16)).toUpper());
    for (int nibble=1;nibble <= fullNibbles;nibble++)
    {
       int temp = BuildUpWord(nibble*4+highestOrderNibble-1,highestOrderNibble+(nibble-1)*4);
       returnValue.append(QString("%1").arg(temp,0,16).toUpper());
    }
    return returnValue;
}
