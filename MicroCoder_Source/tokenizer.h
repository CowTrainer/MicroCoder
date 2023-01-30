#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <QTextStream>
#include <memory>

class Tokenizer;

class Token
{
public:
   enum ETokenType 
   {
      ENDOFFILE,
      STRING,
      NUMBER,
      RESERVED,
      SEMICOLON = ';',
      COMMA = ',',
      BLANK = '~',
      COMMENT = '#'
   };

private:
   ETokenType m_eType;
   QString m_sToken;
   int m_Number;
   friend class Tokenizer;
   
public:
   Token(ETokenType e=STRING);
   ETokenType Type(void) const;
   void SetType(ETokenType t){m_eType=t;}
   const QString & Value(void) const;
   int NumValue(void) const;
   Token & operator=(const Token & rhs);
   bool operator==(Token const & lhs) const{return m_eType ==lhs.m_eType;}
   bool IsReserved(void) const;
};

struct ParseError
{
    QString s;
    int lineNo;
    ParseError(QString const & str, int n):s(str),lineNo(n){}
};

class CStringStream : public QTextStream
{
private:
   qint64 lastpos;
public:
   CStringStream(QString & string)
      : QTextStream(&string,QIODevice::ReadOnly)
   {
      ;
   }
   QChar get(void);
   void putback(QChar c);
};

#define LINE_LENGTH 512

class Tokenizer
{
private:
   //Data members are private to the tokenizer
   QString lineString;
   QTextStream & m_isStream;   //Use value semantics for the input stream
   CStringStream * m_pStringStream;
   int m_nLineNumber;
   Token m_tCurToken;

private:
   //Member functions used only in the tokenizer
   void EatComment(void);
   void HandleGrammarCharacter(QChar c);
   void BuildPlainString(bool isNumber=false);
   bool IsGrammarCharacter(QChar c);
   void MakeStringNumber(char const * format);

public:
   //Tokenizer exports only one constructor, Get and Next
   Tokenizer (QTextStream & stream);
   const Token & GetToken(void) const;
   void NextToken(void);
   int LineNumber(void);
   void Throw(QString const & s);
};

QTextStream & operator<<(QTextStream & out, Token const & token);

/******************************************************************************
   INLINE MEMBER AND HELPER FUNCTION DEFINITIONS
******************************************************************************/
inline Token::Token(ETokenType e) 
   : m_eType(e), m_sToken("")
{
   ;
}

inline Token::ETokenType Token::Type(void) const
{
   return m_eType;
}

inline const QString & Token::Value(void) const
{
   return m_sToken;
}

inline int Token::NumValue(void) const
{
   return m_Number;
}

inline Token & Token::operator=(const Token & rhs)
{
   m_eType = rhs.m_eType;
   m_sToken = rhs.m_sToken;
   m_Number = rhs.m_Number;
   return *this;
}

inline Tokenizer::Tokenizer (QTextStream & stream)
   :m_isStream(stream), 
    m_pStringStream(),
    m_nLineNumber(0)
{
   //We need to seed the tokenizer
   NextToken();
}

inline const Token & Tokenizer::GetToken(void) const
{
   return m_tCurToken;
}
inline int Tokenizer::LineNumber(void)
{
   return m_nLineNumber;
}


#endif // TOKENIZER_H
