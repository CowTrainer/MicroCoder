#include "tokenizer.h"

void Tokenizer::Throw(QString const & s)
{
    throw ParseError(s,LineNumber());
}

bool Tokenizer::IsGrammarCharacter(QChar c)
{
   switch (c.toLatin1())
   {
   case ';':
   case ',':
   case '#':
   case '~':
      return true;
   default:
      return false;
   }
}
bool Token::IsReserved(void) const
{
   if (m_eType != Token::STRING)
      return false;

   return (m_sToken == "CONFIGURATION") ||
          (m_sToken == "ENDCONFIGURATION") ||
          (m_sToken == "PROGRAM") ||
          (m_sToken == "ENDPROGRAM") ||
          (m_sToken == "SIGNAL") ||
          (m_sToken == "CONDITION");
}

void Tokenizer::NextToken(void)
{
   //We gobble up file input one line at a time

   for(;;)
   {
      if ((m_pStringStream!=0) && m_pStringStream->atEnd())
      {
         //Means we have a valid, but empty string stream
         //so delete it.  This must be the end of the line.  Since
         //C doesn't care about whitespace, this is not a token.  We just
         //keep going
         delete m_pStringStream;
         m_pStringStream = 0;
      }

      if( m_pStringStream == 0)
      {
         //We need to read a new line from the file and
         //create a string stream for it--IF WE ARE NOT
         //AT THE END OF THE FILE!!!
         if (m_isStream.atEnd())
         {
            m_tCurToken.m_eType = Token::ENDOFFILE;
            return;
         }
         else
         {
            m_nLineNumber++;
            lineString = m_isStream.readLine();
            //We'll check for a bad input stream, and then
            //not bother checking for a bad string stream.
            if (m_isStream.status() != QTextStream::Ok)
               throw QString("Unknown error in input file");
            m_pStringStream = new CStringStream(lineString);
         }
      }

      //Gobble white space and blank lines at this point
      m_pStringStream->skipWhiteSpace();
      if (m_pStringStream->atEnd())
      {
         delete m_pStringStream;
         m_pStringStream = 0;
         continue;
      }
      QChar c;

      c = m_pStringStream->get();

      if (IsGrammarCharacter(c))
      {
         HandleGrammarCharacter(c);
      }
      else if (c.isDigit())
      {
         m_pStringStream->putback(c);
         BuildPlainString(true);
         MakeStringNumber("%d");
      }
      else
      {
         m_pStringStream->putback(c);
         BuildPlainString();
      }
      if (m_tCurToken.m_eType!=Token::COMMENT)
         break;
      EatComment();
   }
}

void Tokenizer::MakeStringNumber(char const * format)
{
   m_tCurToken.m_eType = Token::NUMBER;

   bool success;
   m_tCurToken.m_Number = m_tCurToken.m_sToken.toInt(&success,0);
   if (!success)
      throw QString("Incorrect number format");
}

void Tokenizer::EatComment(void)
{
   m_tCurToken.m_eType = Token::COMMENT;
   while (!m_pStringStream->atEnd())
   {
      m_pStringStream->get();
   }
}

void Tokenizer::HandleGrammarCharacter(QChar c)
{
   m_tCurToken.m_eType = static_cast<Token::ETokenType>(c.toLatin1());
}

void Tokenizer::BuildPlainString(bool isNumber)
{
   m_tCurToken.m_sToken.clear();
   m_tCurToken.m_eType = Token::STRING;
   for(;;)
   {
      if (m_pStringStream->atEnd())
         break;

      QChar c = m_pStringStream->get();

      //Validate new character
      if ((IsGrammarCharacter(c)) ||
          (isNumber && !c.isDigit()))
      {
         m_pStringStream->putback(c);
         break;
      }
      if (c.isSpace())
         break;

      m_tCurToken.m_sToken.append(c);
   }

}

QChar CStringStream::get(void)
{
   lastpos = pos();
   QString temp = read(1);
   return temp.at(0);
}
void CStringStream::putback(QChar )
{
   seek(lastpos);
}

QTextStream & operator<<(QTextStream & out, Token const & token)
{
   switch(token.Type())
   {
   case Token::STRING:
      out << "STRING ";
      out << '"' << token.Value() << '"';
      break;
   case Token::NUMBER:
      out << "NUMBER " << token.Value();
      break;
   case Token::ENDOFFILE:
      out << "END OF FILE";
      break;
   case Token::COMMENT:
      out << "COMMENT";
      break;
   case Token::RESERVED:
      out << "R "<<token.Value();
      break;
   default:
      out << static_cast<char>(token.Type()) << " ";
      break;
   }
   return out;
}

